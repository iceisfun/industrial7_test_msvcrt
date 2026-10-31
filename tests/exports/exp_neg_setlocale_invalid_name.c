/* TC
 * id:    exp_neg_setlocale_invalid_name
 * area:  exports
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.11.1.1: returns a null pointer if the selection cannot be honored, locale unchanged
 */
#include "tc.h"
#include <locale.h>

static const char *volatile bogus = "xx_NONEXISTENT_zz";

TC_CASE(nonexistent_returns_null) { CHECK(setlocale(LC_ALL, bogus) == NULL); CHECK(setlocale(LC_CTYPE, bogus) == NULL); }
TC_CASE(locale_unchanged_C) { setlocale(LC_ALL, bogus); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
TC_CASE(null_query_returns_C) { CHECK_STR(setlocale(LC_CTYPE, NULL), "C"); CHECK_STR(setlocale(LC_NUMERIC, NULL), "C"); }

int main(void) { tc_begin(); TC_RUN(nonexistent_returns_null); TC_RUN(locale_unchanged_C); TC_RUN(null_query_returns_C); return tc_end(); }
