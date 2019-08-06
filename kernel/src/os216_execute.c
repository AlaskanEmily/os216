/* 
 *  Copyright (c) 2017-2019 Emily McDonough. All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * - Products derived from this software may not be called "os216", nor may
 *     "216" appear in their name, without prior written permission of
 *     the copyright holders.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "os216_execute.h"
#include "os216_malloc.h"
#include "os216_nano_fatal.h"

#include "orl.h"
#include "exeelf.h"

/* Needed for the SEEK_* macros */
#include <stdio.h>
#include <string.h>

/*****************************************************************************/
/* Routines for reading a file as memory.
 * These are the callbacks for liborl. */

struct cli_file{
    const unsigned char *data;
    unsigned length, index;
};

/*****************************************************************************/

static OS216_NANO_NORETURN void os216_invalid_whence(int whence){
    char buffer[60];
    snprintf(buffer,
        sizeof(buffer)-2,
        "Invalid whence from ORL of %i",
        whence);
    buffer[sizeof(buffer)-1] = 0;
    OS216_Nano_Fatal(buffer);
}

/*****************************************************************************/

static const char *os216_whence_name(int whence){
#define OS216_WHENCE_STR_CASE(X) case X: return #X ;
    switch(whence){
        OS216_WHENCE_STR_CASE(SEEK_SET);
        OS216_WHENCE_STR_CASE(SEEK_CUR);
        OS216_WHENCE_STR_CASE(SEEK_END);
        default: return "Invalid whence";
    }
#undef OS216_WHENCE_STR_CASE
}

/*****************************************************************************/

static OS216_NANO_NORETURN void os216_overflow_seek(int whence){
    char buffer[40];
    sprintf(buffer, "Seek past end from ORL in %s", os216_whence_name(whence));
    OS216_Nano_Fatal(buffer);
}

/*****************************************************************************/

static OS216_NANO_NORETURN void os216_underflow_seek(int whence){
    char buffer[40];
    sprintf(buffer, "Seek before start from ORL in %s",
        os216_whence_name(whence));
    OS216_Nano_Fatal(buffer);
}

/*****************************************************************************/

static long os216_cli_seek(orl_file_id file_void, long amount, int whence){
    struct cli_file *const file = (struct cli_file *)file_void;
    long new_index;
    switch(whence){
        case SEEK_SET:
            new_index = amount;
            break;
        case SEEK_CUR:
            new_index = amount + (long)file->index;
            break;
        case SEEK_END:
            new_index = file->length + amount;
            break;
        default:
            os216_invalid_whence(whence);
    }
    if(new_index > file->length)
        os216_overflow_seek(whence);
    if(new_index < 0)
        os216_underflow_seek(whence);
    return file->index;
}

/*****************************************************************************/

static void *os216_cli_read(orl_file_id file_void, size_t len){
    struct cli_file *const file = (struct cli_file *)file_void;
    const unsigned at = file->index;
    void *const data = memcpy(malloc(len), file->data + at, len);
    file->index = at + len;
    return data;
}

/*****************************************************************************/

struct os216_find_symbol_result{
    const char *name;
    orl_symbol_value symbol;
    orl_sec_handle section;
    orl_return found;
};

/*****************************************************************************/

static orl_return os216_find_symbol(const orl_symbol_handle sym, void *arg){
    struct os216_find_symbol_result *const result = arg;
    const char *const name = ORLSymbolGetName(sym);
    if(strcmp(name, result->name) == 0){
        const orl_return symbol_value_ok =
            ORLSymbolGetValue(sym, &(result->symbol));
        if(symbol_value_ok != ORL_OKAY){
            OS216_Nano_Fatal("Could not get symbol value from ORL");
        }
        result->section = ORLSymbolGetSecHandle(sym);
        result->found = ORL_TRUE;
        return ORL_FALSE;
    }
    else{
        return ORL_TRUE;
    }
}

/*****************************************************************************/

typedef void(*os216_start_func_t)(void);

/*****************************************************************************/

void OS216_Execute(const void *file_data, unsigned file_len){
    struct cli_file file;
    struct orl_funcs funcs;
    orl_file_format format;
    orl_handle handle;
    orl_file_handle file_handle;
    
    file.data = file_data;
    file.length = file_len;
    file.index = 0;
    
    funcs.cli_alloc = malloc;
    funcs.cli_free = free;
    funcs.cli_read = os216_cli_read;
    funcs.cli_seek = os216_cli_seek;
    
    handle = ORLInit(&funcs);
    
    format = ORLFileIdentify(handle, &file);
    if(format == ORL_UNRECOGNIZED_FORMAT){
        OS216_Nano_Fatal("Invalid executable");
    }
    
    file_handle = ORLFileInit(handle, &file, format);
    if(file_handle == NULL){
        OS216_Nano_Fatal("ORL could not open file");
    }
    else{
        const orl_sec_handle symbol_table = ORLFileGetSymbolTable(file_handle);
        struct os216_find_symbol_result result;
        result.name = "start";
        result.found = ORL_FALSE;
        ORLSymbolSecScan(symbol_table, os216_find_symbol, &result);
        if(result.found != ORL_TRUE){
            OS216_Nano_Fatal("ORL could not find entry point");
        }
        else{
            unsigned char *contents;
            const orl_return contents_ok =
                ORLSecGetContents(result.section, &contents);
            if(contents_ok != ORL_TRUE){
                OS216_Nano_Fatal("ORL could not get code section");
            }
            {
                os216_start_func_t start;
                void *at = contents + result.symbol;
                
                /* EEEEEEVIL! */
                memcpy(&start, &at, sizeof(void*));
                
                /* You only yolo once. */
                start();
            }
        }
    }
}
