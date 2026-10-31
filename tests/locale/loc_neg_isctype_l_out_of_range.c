/* TC
 * id:    loc_neg_isctype_l_out_of_range
 * area:  locale
 * funcs: _isctype_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isctype_l (domain -1..255 for the narrow functions)
 * note:  PROBE: values outside -1..255 are undefined by docs; expecting release CRTs to return 0 without invoking the invalid-parameter handler. EOF (-1) must give 0.
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
static const char *volatile v_c = "C";

TC_CASE(wide_value_in_narrow_isctype_zero) { volatile int c = 0x1234; CHECK_EQ_I(_isctype_l(c, _ALPHA, loc), 0); }
TC_CASE(eof_zero) { volatile int c = -1; CHECK_EQ_I(_isctype_l(c, _ALPHA | _DIGIT, loc), 0); }
TC_CASE(iph_not_invoked)
{
    volatile int c = 0x1234, e = -1;
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    (void)_isctype_l(c, _ALPHA, loc); (void)_isctype_l(e, _ALPHA | _DIGIT, loc);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, v_c);
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create C locale\n"); return 1; }
    TC_RUN(wide_value_in_narrow_isctype_zero); TC_RUN(eof_zero); TC_RUN(iph_not_invoked);
    _free_locale(loc);
    return tc_end();
}
