/* TC
 * id:    loc_mb_cur_max_sbcs
 * area:  locale
 * funcs: ___mb_cur_max_func setlocale
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs MB_CUR_MAX (1 for single-byte locales)
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

TC_CASE(c_locale_1) { setlocale(LC_ALL, "C"); CHECK_EQ_I(___mb_cur_max_func(), 1); }
TC_CASE(cp1252_1)
{
    volatile const char *n = "English_United States.1252";
    if (!setlocale(LC_ALL, (const char *)n)) TC_SKIP("locale unavailable");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    setlocale(LC_ALL, "C");
}
TC_CASE(back_to_c_1) { setlocale(LC_ALL, "C"); CHECK_EQ_I(___mb_cur_max_func(), 1); }
int main(void) { tc_begin(); TC_RUN(c_locale_1); TC_RUN(cp1252_1); TC_RUN(back_to_c_1); return tc_end(); }
