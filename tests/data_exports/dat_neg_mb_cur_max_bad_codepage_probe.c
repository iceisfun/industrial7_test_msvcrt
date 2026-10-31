/* TC
 * id:    dat_neg_mb_cur_max_bad_codepage_probe
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (code page must be valid and installed)
 * note:  Probe: setlocale(LC_ALL, ".99999") (nonexistent code page) returns NULL and leaves the C locale / MB_CUR_MAX 1 untouched.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_CASE(invalid_codepage_rejected)
{
    CHECK(setlocale(LC_ALL, ".99999") == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(invalid_codepage_rejected); setlocale(LC_ALL, "C"); return tc_end(); }
