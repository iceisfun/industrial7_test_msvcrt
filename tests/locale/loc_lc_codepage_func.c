/* TC
 * id:    loc_lc_codepage_func
 * area:  locale
 * funcs: ___lc_codepage_func setlocale
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs setlocale (locale name "language_country.codepage")
 * note:  C-locale value is probed in loc_probe_lc_codepage_c_locale.
 */
#include "tc.h"
#include <locale.h>

static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_932 = ".932";
static const char *volatile n_dot1252 = ".1252";
static const char *volatile n_c = "C";

TC_CASE(cp1252)
{
    if (!setlocale(LC_ALL, n_1252)) { setlocale(LC_ALL, n_c); TC_SKIP("English_United States.1252 unavailable"); }
    CHECK_EQ_U(___lc_codepage_func(), 1252);
    setlocale(LC_ALL, n_c);
}
TC_CASE(cp932)
{
    if (!setlocale(LC_ALL, n_932)) { setlocale(LC_ALL, n_c); TC_SKIP(".932 unavailable"); }
    CHECK_EQ_U(___lc_codepage_func(), 932);
    setlocale(LC_ALL, n_c);
}
TC_CASE(explicit_dot_1252)
{
    if (!setlocale(LC_ALL, n_dot1252)) { setlocale(LC_ALL, n_c); TC_SKIP(".1252 unavailable"); }
    CHECK_EQ_U(___lc_codepage_func(), 1252);
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin();
    TC_RUN(cp1252); TC_RUN(cp932); TC_RUN(explicit_dot_1252);
    setlocale(LC_ALL, "C");
    return tc_end();
}
