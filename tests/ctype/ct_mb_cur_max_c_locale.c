/* TC
 * id:    ct_mb_cur_max_c_locale
 * area:  ctype
 * funcs: ___mb_cur_max_func setlocale
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1 MB_CUR_MAX; MS docs MB_CUR_MAX
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_CASE(equals_1) { CHECK_EQ_I(___mb_cur_max_func(), 1); }
TC_CASE(stable_after_setlocale_C_roundtrip)
{
    CHECK(setlocale(LC_ALL, "C") != NULL);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    CHECK(setlocale(LC_CTYPE, "C") != NULL);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(equals_1); TC_RUN(stable_after_setlocale_C_roundtrip); return tc_end(); }
