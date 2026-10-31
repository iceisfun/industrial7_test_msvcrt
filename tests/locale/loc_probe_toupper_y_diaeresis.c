/* TC
 * id:    loc_probe_toupper_y_diaeresis
 * area:  locale
 * funcs: _toupper_l _tolower_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _toupper_l; cp1252 code page chart
 * note:  PROBE: letters in cp1252 0x80..0x9F (S/s caron 0x8A/0x9A, Y diaeresis 0x9F) are case-mapped via NLS; yuml 0xFF uppercases to 0x9F. Each is its own case.
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

TC_CASE(yuml_to_Yuml_0x9F) { volatile int c = 0xFF; CHECK_EQ_I(_toupper_l(c, loc), 0x9F); }
TC_CASE(Yuml_to_yuml) { volatile int c = 0x9F; CHECK_EQ_I(_tolower_l(c, loc), 0xFF); }
TC_CASE(scaron_pair)
{
    volatile int up = 0x8A, lo = 0x9A;
    CHECK_EQ_I(_tolower_l(up, loc), 0x9A);
    CHECK_EQ_I(_toupper_l(lo, loc), 0x8A);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, v_n);
    if (!loc) { tc_write("TC SKIP " TC_TEST_ID "::__locale unavailable\n"); return 0; }
    TC_RUN(yuml_to_Yuml_0x9F); TC_RUN(Yuml_to_yuml); TC_RUN(scaron_pair);
    _free_locale(loc);
    return tc_end();
}
