/* TC
 * id:    loc_mb_cur_max_dbcs
 * area:  locale
 * funcs: ___mb_cur_max_func setlocale
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs MB_CUR_MAX, setlocale (code page argument)
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static void one(const char *name)
{
    volatile const char *n = name;
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("code page rejected"); }
    CHECK_EQ_I(___mb_cur_max_func(), 2);
    setlocale(LC_ALL, "C");
}
TC_CASE(cp932_2) { one(".932"); }
TC_CASE(cp936_2) { one(".936"); }
TC_CASE(cp949_2) { one(".949"); }
TC_CASE(cp950_2) { one(".950"); }
int main(void) { tc_begin(); TC_RUN(cp932_2); TC_RUN(cp936_2); TC_RUN(cp949_2); TC_RUN(cp950_2); return tc_end(); }
