/* TC
 * id:    loc_tolower_toupper_c
 * area:  locale
 * funcs: tolower toupper _tolower _toupper towlower towupper
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.2.1, 7.4.2.2, 7.29.3.2; MS docs tolower, _tolower, towlower
 * note:  _tolower/_toupper are only defined for letters of the proper case; they are applied only to such letters.
 */
#include "tc.h"
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
#include <wctype.h>

TC_CASE(tolower_ascii)
{
    int i; for (i = 'A'; i <= 'Z'; i++) { volatile int c = i; CHECK_EQ_I(tolower(c), i + 32); }
    for (i = 'a'; i <= 'z'; i++) { volatile int c = i; CHECK_EQ_I(tolower(c), i); }
}
TC_CASE(toupper_ascii)
{
    int i; for (i = 'a'; i <= 'z'; i++) { volatile int c = i; CHECK_EQ_I(toupper(c), i - 32); }
    for (i = 'A'; i <= 'Z'; i++) { volatile int c = i; CHECK_EQ_I(toupper(c), i); }
}
TC_CASE(non_letters_unchanged)
{
    int i;
    for (i = 0; i < 128; i++) { volatile int c = i; if ((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z')) continue;
        if (tolower(c) != i || toupper(c) != i) { tc_fail_ints(__FILE__, __LINE__, "tolower/toupper identity", "ok", i, 0); break; } }
}
TC_CASE(underscore_variants_on_letters)
{
    int i;
    for (i = 'A'; i <= 'Z'; i++) { volatile int c = i; CHECK_EQ_I(_tolower(c), i + 32); }
    for (i = 'a'; i <= 'z'; i++) { volatile int c = i; CHECK_EQ_I(_toupper(c), i - 32); }
}
TC_CASE(wide_ascii)
{
    wint_t i;
    for (i = L'A'; i <= L'Z'; i++) { volatile wint_t c = i; CHECK_EQ_I(towlower(c), i + 32); CHECK_EQ_I(towupper(c), i); }
    for (i = L'a'; i <= L'z'; i++) { volatile wint_t c = i; CHECK_EQ_I(towupper(c), i - 32); CHECK_EQ_I(towlower(c), i); }
    { volatile wint_t d = L'5', p = L'!'; CHECK_EQ_I(towlower(d), L'5'); CHECK_EQ_I(towupper(p), L'!'); }
}
int main(void) { tc_begin(); TC_RUN(tolower_ascii); TC_RUN(toupper_ascii); TC_RUN(non_letters_unchanged); TC_RUN(underscore_variants_on_letters); TC_RUN(wide_ascii); return tc_end(); }
