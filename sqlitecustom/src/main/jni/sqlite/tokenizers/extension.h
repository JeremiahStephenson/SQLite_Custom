
#ifndef EXTENSION_H
#define EXTENSION_H

#include "../fts3_tokenizer.h"
#include "../../fts3_html_tokenizer.h"

#define HTML_NAME "HTMLTokenizer"
#define CHARACTER_NAME "character"

void set_html_tokenizer_module(sqlite3_tokenizer_module const **ppModule);
void set_character_tokenizer_module(const sqlite3_tokenizer_module **ppModule);

int registerExtensionTokenizer(sqlite3 *db, const char *zName, const sqlite3_tokenizer_module *p);
void androidLog(char const *log);

#endif