/* TC
 * id:    dat_neg_mb_cur_max_utf16_utf7_old_probe
 * area:  data_exports
 * kind:  negative
 * funcs: ___mb_cur_max_func setlocale
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   (UCRT docs; pre-UCRT unverified) MS docs setlocale: "UTF-7 and UTF-16 are not supported as the code page" (CRT locale names)
 * note:  PROBE: on pre-UCRT CRTs code pages 1200/1201 (UTF-16) and 65000 (UTF-7) cannot be selected: setlocale returns NULL and the C locale is unchanged.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char *volatile loc_all_1200 = ".1200";
static const char *volatile loc_all_1201 = ".1201";
static const char *volatile loc_all_7 = ".65000";

TC_CASE(utf16le_all)
{
    CHECK(setlocale(LC_ALL, loc_all_1200) == NULL);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
TC_CASE(utf16be_all) { CHECK(setlocale(LC_ALL, loc_all_1201) == NULL); CHECK_EQ_I(___mb_cur_max_func(), 1); }
TC_CASE(utf7_all) { CHECK(setlocale(LC_ALL, loc_all_7) == NULL); CHECK_EQ_I(___mb_cur_max_func(), 1); }
TC_CASE(utf16_ctype)
{
    setlocale(LC_ALL, "C");
    CHECK(setlocale(LC_CTYPE, loc_all_1200) == NULL);
    CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
}
TC_CASE(restore)
{
    CHECK_STR(setlocale(LC_ALL, "C"), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(utf16le_all); TC_RUN(utf16be_all); TC_RUN(utf7_all); TC_RUN(utf16_ctype); TC_RUN(restore);
    setlocale(LC_ALL, "C");
    return tc_end();
}
