
#include "../../fts3_html_tokenizer.h"

#define HTML_NAME "HTMLTokenizer"

#define UNICODE0_DLL_EXPORTED __attribute__((__visibility__("default")))

void sqlite3Fts3UnicodeTokenizer(sqlite3_tokenizer_module const **ppModule);

struct sqlite3_api_routines;

UNICODE0_DLL_EXPORTED int sqlite3_extension_init(
      sqlite3 *db,          /* The database connection */
      char **pzErrMsg,      /* Write error messages here */
      const struct sqlite3_api_routines *pApi  /* API methods */
      );
