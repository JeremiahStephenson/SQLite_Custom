//
//  character_tokenizer.c
//
//  Created by Jeremiah Stephenson on 1/29/15.
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
// Implementation of the "simple" full-text-search tokenizer.

#include "../../sqlite3.h"
#include <ctype.h> //for tolower
#include <string.h> //for memset
#include "xml_tokenizer.h"

#include <android/log.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct xml_tokenizer {
    sqlite3_tokenizer base;
} xml_tokenizer;

typedef struct xml_tokenizer_cursor {
    sqlite3_tokenizer_cursor base;
    const char *pInput; // input we are tokenizing
    int nBytes;         // size of the input
    int iPosition;      // current position in pInput
    int iToken;         // index of next token to be returned
    char *pToken;       // storage for current token
} xml_tokenizer_cursor;

static int xmlCreate(int argc, const char * const *argv, sqlite3_tokenizer **ppTokenizer) {

    xml_tokenizer *t;
    t = (xml_tokenizer *) sqlite3_malloc(sizeof(*t));
    if (t == NULL) {
        return SQLITE_NOMEM;
    }
    memset(t, 0, sizeof(*t));
    
    *ppTokenizer = &t->base;
    return SQLITE_OK;
}

static int xmlDestroy(sqlite3_tokenizer *pTokenizer) {

    sqlite3_free(pTokenizer);
    return SQLITE_OK;
}

static int xmlOpen(
                   sqlite3_tokenizer *pTokenizer,         /* The tokenizer */
                   const char *pInput, int nBytes,        /* String to be tokenized */
                   sqlite3_tokenizer_cursor **ppCursor    /* OUT: Tokenization cursor */
                   ) {

    xml_tokenizer_cursor *c;
    if (pInput == 0) {
        nBytes = 0;
    } else if (nBytes < 0) {
        nBytes = (int)strlen(pInput);
    }
    c = (xml_tokenizer_cursor *) sqlite3_malloc(sizeof(*c));
    if (c == NULL) {
        return SQLITE_NOMEM;
    }
    c->iToken = c->iPosition = 0;
    c->pToken = NULL;
    c->nBytes = nBytes;
    c->pInput = pInput;
    *ppCursor = &c->base;
    return SQLITE_OK;
}

static int xmlClose(sqlite3_tokenizer_cursor *pCursor) {

    xml_tokenizer_cursor *c = (xml_tokenizer_cursor *) pCursor;
    
    if (c->pToken != NULL){
        sqlite3_free(c->pToken);
        c->pToken = NULL;
    }

    sqlite3_free(c);
    return SQLITE_OK;
}

static int xmlNext(
                   sqlite3_tokenizer_cursor *pCursor, /* Cursor returned by cusOpen */
                   const char **ppToken,               /* OUT: *ppToken is the token text */
                   int *pnBytes,                       /* OUT: Number of bytes in token */
                   int *piStartOffset,                 /* OUT: Starting offset of token */
                   int *piEndOffset,                   /* OUT: Ending offset of token */
                   int *piPosition                     /* OUT: Position integer of token */
                   ) {

    xml_tokenizer_cursor *c = (xml_tokenizer_cursor *) pCursor;
    if (c->pToken != NULL) {
        sqlite3_free(c->pToken);
        c->pToken = NULL;
    }
    
    if (c->iPosition >= c->nBytes) {
        return SQLITE_DONE;
    }
    
    int length = 0; // the size of current xml, which can be at most 4 bytes

    //__android_log_print(ANDROID_LOG_VERBOSE, "xml Token Test", "%s", c->pInput);

    const char* token = &(c->pInput[c->iPosition]);
    *piStartOffset = c->iPosition;

    int offset = 0;
    bool inside = false;

    // find the beginning of next utf8 character
    //c->iPosition++;
    while (c->iPosition < c->nBytes) {
        char byte = c->pInput[c->iPosition];

        if (isspace(byte) || byte == 0x3C || byte == 0x3E || inside) {
            c->iPosition++;

            if (byte == 0x3C) {
                __android_log_print(ANDROID_LOG_VERBOSE, "xml Token Test", "Inside: %c", byte);
                inside = true;
            } else if (byte == 0x3E) {
                __android_log_print(ANDROID_LOG_VERBOSE, "xml Token Test", "Outside: %c", byte);
                inside = false;
            }

            if (length == 0) {
                offset++;
                continue;
            }

            break;
        }

        length++;
        c->iPosition++;
    }
    
    c->pToken = (char *)sqlite3_malloc(length+1);
    if (c->pToken == NULL) {
        return SQLITE_NOMEM;
    }
    
    c->pToken[length] = 0;
    memcpy(c->pToken, token + offset, length);
    
    for (int i = 0; i < length; ++i) {
        unsigned char byte = c->pToken[i];

        //__android_log_print(ANDROID_LOG_VERBOSE, "xml Token Test", "%c", byte);

        if (!isspace(byte) && !(byte == 0x3C) && !(byte == 0x3E)) {
            // ascii character, make it case-insensitive
            c->pToken[i] = tolower(byte);
        }
    }
    
    *ppToken = c->pToken;
    *pnBytes = length;
    
    *piEndOffset = *piStartOffset+length;
    *piPosition = c->iToken++;

    __android_log_print(ANDROID_LOG_VERBOSE, "xml Token Test", "%s, %i, %i, %i, %i", c->pToken, *pnBytes, *piStartOffset, *piEndOffset, *piPosition);

    return SQLITE_OK;
}

static const sqlite3_tokenizer_module xmlTokenizerModule = {
    0, // version number
    xmlCreate,
    xmlDestroy,
    xmlOpen,
    xmlClose,
    xmlNext,
};

void get_xml_tokenizer_module(const sqlite3_tokenizer_module **ppModule){
    *ppModule = &xmlTokenizerModule;
}
