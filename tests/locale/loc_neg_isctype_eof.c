/* TC
 * id:    loc_neg_isctype_eof
 * area:  locale
 * funcs: _isalpha_l _isalnum_l _isdigit_l _isspace_l _isprint_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.4p1 (EOF is a valid argument); MS docs isalpha
 * note:  EOF is a valid argument and is not in any class.
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
#include <stdio.h>

static _locale_t loc;
static const char *volatile v_c = "C";
static volatile int v_eof = EOF;

TC_CASE(isalpha_l_eof) { CHECK_EQ_I(_isalpha_l(v_eof, loc), 0); CHECK_EQ_I(_isalnum_l(v_eof, loc), 0); }
TC_CASE(isdigit_l_eof) { CHECK_EQ_I(_isdigit_l(v_eof, loc), 0); }
TC_CASE(isspace_l_eof) { CHECK_EQ_I(_isspace_l(v_eof, loc), 0); }
TC_CASE(isprint_l_eof) { CHECK_EQ_I(_isprint_l(v_eof, loc), 0); }

int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, v_c);
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create C locale\n"); return 1; }
    TC_RUN(isalpha_l_eof); TC_RUN(isdigit_l_eof); TC_RUN(isspace_l_eof); TC_RUN(isprint_l_eof);
    _free_locale(loc);
    return tc_end();
}
