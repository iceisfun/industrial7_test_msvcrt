/* TC
 * id:    loc_neg_ctype_l_null_locale
 * area:  locale
 * funcs: _isalpha_l _isdigit_l _tolower_l _toupper_l setlocale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isalpha_l (locale parameter)
 * note:  PROBE: a NULL _locale_t means the current locale (same results as isalpha/tolower) and must not crash.
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

static const char *volatile v_n = "English_United States.1252";
static const char *volatile v_c = "C";

TC_CASE(isalpha_l_null)
{
    volatile int a = 'A', d = '5';
    CHECK(_isalpha_l(a, NULL) != 0);
    CHECK(_isalpha_l(d, NULL) == 0);
    CHECK(_isdigit_l(d, NULL) != 0);
}
TC_CASE(tolower_l_null)
{
    volatile int a = 'A', b = 'b';
    CHECK_EQ_I(_tolower_l(a, NULL), 'a');
    CHECK_EQ_I(_toupper_l(b, NULL), 'B');
}
TC_CASE(follows_global_after_setlocale_1252)
{
    volatile int up = 0xC0, lo = 0xE0;
    if (!setlocale(LC_ALL, v_n)) TC_SKIP("locale unavailable");
    CHECK(_isalpha_l(up, NULL) != 0);
    CHECK_EQ_I(_tolower_l(up, NULL), 0xE0);
    CHECK_EQ_I(_toupper_l(lo, NULL), 0xC0);
    setlocale(LC_ALL, v_c);
}
int main(void) { tc_begin(); TC_RUN(isalpha_l_null); TC_RUN(tolower_l_null); TC_RUN(follows_global_after_setlocale_1252); return tc_end(); }
