/* TC
 * id:    dat_neg_mb_cur_max_utf8_old_probe
 * area:  data_exports
 * kind:  negative
 * funcs: ___mb_cur_max_func setlocale
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs setlocale (UTF-8 code page support added in Windows 10 1803 UCRT)
 * note:  PROBE: pre-UCRT CRTs do not accept code page 65001 in setlocale (return NULL, locale unchanged, MB_CUR_MAX stays 1).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char *volatile l1 = ".65001";
static const char *volatile l2 = "English_United States.65001";

TC_CASE(dot_65001) { CHECK(setlocale(LC_ALL, l1) == NULL); }
TC_CASE(named_65001) { CHECK(setlocale(LC_CTYPE, l2) == NULL); }
TC_CASE(mb_cur_max_unchanged) { CHECK_EQ_I(___mb_cur_max_func(), 1); }
int main(void)
{
    tc_begin();
    TC_RUN(dot_65001); TC_RUN(named_65001); TC_RUN(mb_cur_max_unchanged);
    setlocale(LC_ALL, "C");
    return tc_end();
}
