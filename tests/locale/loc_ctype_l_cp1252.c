/* TC
 * id:    loc_ctype_l_cp1252
 * area:  locale
 * funcs: _isalpha_l _isupper_l _islower_l _isprint_l _ispunct_l _tolower_l _toupper_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs setlocale (language_country.codepage); Latin-1 repertoire shared with cp1252
 * note:  Does not assert toupper(0xFF) (0x9F in cp1252): see loc_probe_toupper_y_diaeresis. Skipped if the locale cannot be created.
 */
#include "tc.h"
#include <locale.h>
#include <ctype.h>
/* mingw's ctype.h defines these as inline macros; undefine so the DLL's exports are what is called */
#undef _isalpha_l
#undef _isupper_l
#undef _islower_l
#undef _isdigit_l
#undef _isxdigit_l
#undef _isspace_l
#undef _ispunct_l
#undef _isalnum_l
#undef _isprint_l
#undef _isgraph_l
#undef _iscntrl_l
#undef _tolower
#undef _toupper

static _locale_t loc;
static const char *volatile v_n = "English_United States.1252";

TC_CASE(upper_range)
{
    int i;
    for (i = 0xC0; i <= 0xDE; i++) { volatile int c = i; if (i == 0xD7) continue;
        if (!_isupper_l(c, loc) || !_isalpha_l(c, loc) || _islower_l(c, loc)) { tc_fail_ints(__FILE__, __LINE__, "upper alpha !lower", "ok", i, 0); break; } }
}
TC_CASE(lower_range)
{
    int i;
    for (i = 0xDF; i <= 0xFF; i++) { volatile int c = i; if (i == 0xF7) continue;
        if (!_islower_l(c, loc) || !_isalpha_l(c, loc) || _isupper_l(c, loc)) { tc_fail_ints(__FILE__, __LINE__, "lower alpha !upper", "ok", i, 0); break; } }
}
TC_CASE(multiplication_division_signs_not_alpha)
{
    volatile int m = 0xD7, d = 0xF7;
    CHECK(_isalpha_l(m, loc) == 0); CHECK(_isalpha_l(d, loc) == 0);
    CHECK(_ispunct_l(m, loc) != 0); CHECK(_ispunct_l(d, loc) != 0);
    CHECK(_isprint_l(m, loc) != 0); CHECK(_isprint_l(d, loc) != 0);
}
TC_CASE(case_mapping_pairs)
{
    int i;
    for (i = 0xC0; i <= 0xDE; i++) { volatile int u = i, l = i + 0x20; if (i == 0xD7) continue;
        if (_tolower_l(u, loc) != l || _toupper_l(l, loc) != u) { tc_fail_ints(__FILE__, __LINE__, "case pair", "ok", i, 0); break; } }
}
TC_CASE(sharp_s_unchanged_by_toupper)
{
    volatile int ss = 0xDF, div = 0xF7, mul = 0xD7;
    CHECK_EQ_I(_toupper_l(ss, loc), 0xDF);
    CHECK_EQ_I(_toupper_l(div, loc), 0xF7);
    CHECK_EQ_I(_tolower_l(mul, loc), 0xD7);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, v_n);
    if (!loc) { tc_write("TC SKIP " TC_TEST_ID "::__locale unavailable\n"); return 0; }
    TC_RUN(upper_range); TC_RUN(lower_range); TC_RUN(multiplication_division_signs_not_alpha);
    TC_RUN(case_mapping_pairs); TC_RUN(sharp_s_unchanged_by_toupper);
    _free_locale(loc);
    return tc_end();
}
