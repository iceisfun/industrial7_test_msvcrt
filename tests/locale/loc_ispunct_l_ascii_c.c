/* TC
 * id:    loc_ispunct_l_ascii_c
 * area:  locale
 * funcs: _ispunct_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.1.9; MS docs ispunct, _ispunct_l
 * note:  Punctuation is every printable ASCII char that is neither alnum nor space.
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

static const char *volatile v_c = "C";
static const char punct[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

static int in_set(int c) { const char *p; for (p = punct; *p; p++) if (*p == c) return 1; return 0; }

TC_CASE(punct_exact_set)
{
    _locale_t l = _create_locale(LC_ALL, v_c);
    int i;
    if (!l) TC_SKIP("no C locale");
    for (i = 0; i < 128; i++) {
        volatile int c = i;
        if ((_ispunct_l(c, l) != 0) != in_set(i)) { tc_fail_ints(__FILE__, __LINE__, "_ispunct_l", "in_set", _ispunct_l(c, l) != 0, in_set(i)); break; }
    }
    _free_locale(l);
}
TC_CASE(not_punct_alnum_space_ctrl)
{
    _locale_t l = _create_locale(LC_ALL, v_c);
    volatile int c;
    if (!l) TC_SKIP("no C locale");
    c = 'a'; CHECK(_ispunct_l(c, l) == 0);
    c = 'Z'; CHECK(_ispunct_l(c, l) == 0);
    c = '0'; CHECK(_ispunct_l(c, l) == 0);
    c = ' '; CHECK(_ispunct_l(c, l) == 0);
    c = '\n'; CHECK(_ispunct_l(c, l) == 0);
    c = 0; CHECK(_ispunct_l(c, l) == 0);
    c = 127; CHECK(_ispunct_l(c, l) == 0);
    _free_locale(l);
}
int main(void) { tc_begin(); TC_RUN(punct_exact_set); TC_RUN(not_punct_alnum_space_ctrl); return tc_end(); }
