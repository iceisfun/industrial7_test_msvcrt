/* TC
 * id:    dat_mb_cur_max_c_locale
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs MB_CUR_MAX, setlocale (C locale is single-byte)
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

TC_CASE(c_locale_is_1) { CHECK_EQ_I(___mb_cur_max_func(), 1); CHECK_EQ_I(MB_CUR_MAX, 1); }
TC_CASE(after_setlocale_c_still_1)
{
    CHECK_STR(setlocale(LC_ALL, "C"), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    CHECK_STR(setlocale(LC_CTYPE, "C"), "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
TC_CASE(func_returns_stable_value)
{
    int a = ___mb_cur_max_func(), b = ___mb_cur_max_func(), i;
    for (i = 0; i < 100; i++) b = ___mb_cur_max_func();
    CHECK_EQ_I(a, b); CHECK(a >= 1);
}
int main(void) { tc_begin(); TC_RUN(c_locale_is_1); TC_RUN(after_setlocale_c_still_1); TC_RUN(func_returns_stable_value); setlocale(LC_ALL, "C"); return tc_end(); }
