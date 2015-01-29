#ifndef _FTS3_UNICODE_SN_H
#define _FTS3_UNICODE_SN_H

#include "fts3_tokenizer.h"

#define TOKENIZER_NAME	"unicodesn"
#define CHARACTER_NAME  "character"
#define XML_NAME  "xml"

#define UNICODE0_DLL_EXPORTED __attribute__((__visibility__("default")))

struct sqlite3_api_routines;

void sqlite3Fts3UnicodeSnTokenizer(sqlite3_tokenizer_module const **ppModule);

UNICODE0_DLL_EXPORTED int sqlite3_extension_init(
      sqlite3 *db,          /* The database connection */
      char **pzErrMsg,      /* Write error messages here */
      const struct sqlite3_api_routines *pApi  /* API methods */
      );

UNICODE0_DLL_EXPORTED int sqlite3_extension_init_character(
    sqlite3 *db,          /* The database connection */
    char **pzErrMsg,      /* Write error messages here */
    const struct sqlite3_api_routines *pApi  /* API methods */
    );

UNICODE0_DLL_EXPORTED int sqlite3_extension_init_xml(
    sqlite3 *db,          /* The database connection */
    char **pzErrMsg,      /* Write error messages here */
    const struct sqlite3_api_routines *pApi  /* API methods */
    );


#endif /* _FTS3_UNICODE0_H */
