/* TC
 * id:    mb_mblen_basic
 * area:  mbcs
 * funcs: mblen setlocale
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.7.1; MS docs mblen
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>

TC_CASE(c_locale) { volatile const char *a = "a", *e = ""; CHECK_EQ_I(mblen((const char *)a, 1), 1); CHECK_EQ_I(mblen((const char *)e, 1), 0); }
TC_CASE(null_stateless) { volatile const char *n = NULL; CHECK_EQ_I(mblen((const char *)n, 0), 0); }
TC_CASE(cp932)
{
    static const char dbc[] = { (char)0x82, (char)0xA0, 0 };
    volatile const char *p = dbc, *a = "a";
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(mblen((const char *)p, 2), 2);
    CHECK_EQ_I(mblen((const char *)a, 2), 1);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(c_locale); TC_RUN(null_stateless); TC_RUN(cp932); setlocale(LC_ALL, "C"); return tc_end(); }
