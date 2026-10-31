/* TC
 * id:    loc_lc_collate_cp_func
 * area:  locale
 * funcs: ___lc_collate_cp_func ___lc_codepage_func setlocale
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs setlocale (categories are set independently)
 * note:  The collate code page follows LC_COLLATE only; the ctype code page follows LC_CTYPE only.
 */
#include "tc.h"
#include <locale.h>

int __cdecl ___lc_collate_cp_func(void);
static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_932 = ".932";
static const char *volatile n_c = "C";

TC_CASE(collate_1252)
{
    if (!setlocale(LC_CTYPE, n_932) || !setlocale(LC_COLLATE, n_1252)) {
        setlocale(LC_ALL, n_c); TC_SKIP("locales unavailable");
    }
    CHECK_EQ_I(___lc_collate_cp_func(), 1252);
    setlocale(LC_ALL, n_c);
}
TC_CASE(ctype_unaffected_932_via_lc_codepage_func)
{
    if (!setlocale(LC_CTYPE, n_932) || !setlocale(LC_COLLATE, n_1252)) {
        setlocale(LC_ALL, n_c); TC_SKIP("locales unavailable");
    }
    CHECK_EQ_U(___lc_codepage_func(), 932);
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin();
    TC_RUN(collate_1252); TC_RUN(ctype_unaffected_932_via_lc_codepage_func);
    setlocale(LC_ALL, "C");
    return tc_end();
}
