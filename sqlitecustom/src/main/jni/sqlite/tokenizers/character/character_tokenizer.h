//
//  character_tokenizer.h
//
//  Created by Hai Feng Kao on 4/6/13.
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef SQLITE_CHARACTERTOKENIZER_H
#define SQLITE_CHARACTERTOKENIZER_H

#include "../fts3_tokenizer.h"

#define CHARACTER_NAME  "character"

#define UNICODE0_DLL_EXPORTED __attribute__((__visibility__("default")))

void get_character_tokenizer_module(const sqlite3_tokenizer_module **ppModule);

struct sqlite3_api_routines;

UNICODE0_DLL_EXPORTED int sqlite3_extension_init_character(
    sqlite3 *db,          /* The database connection */
    char **pzErrMsg,      /* Write error messages here */
    const struct sqlite3_api_routines *pApi  /* API methods */
    );

#endif