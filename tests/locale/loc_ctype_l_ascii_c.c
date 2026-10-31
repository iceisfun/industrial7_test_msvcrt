/* TC
 * id:    loc_ctype_l_ascii_c
 * area:  locale
 * funcs: _isalpha_l _isdigit_l _isupper_l _islower_l _isspace_l _isxdigit_l _isalnum_l _isprint_l _iscntrl_l _isgraph_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.1; MS docs isalpha, isalpha_l, _create_locale
 * note:  ispunct_l is checked in loc_ispunct_l_ascii_c because msvcrt lacks it. Reference classifications are hand-written.
 */
#include "tc.h"
#include <locale.h>
typedef _locale_t (__cdecl *tc_cl_t)(int, const char *);
typedef void (__cdecl *tc_fl_t)(_locale_t);
static tc_cl_t p_create_locale;
static tc_fl_t p_free_locale;
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

static int r_upper(int c) { return c >= 'A' && c <= 'Z'; }
static int r_lower(int c) { return c >= 'a' && c <= 'z'; }
static int r_alpha(int c) { return r_upper(c) || r_lower(c); }
static int r_digit(int c) { return c >= '0' && c <= '9'; }
static int r_space(int c) { return c == ' ' || (c >= 9 && c <= 13); }
static int r_xdigit(int c) { return r_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
static int r_cntrl(int c) { return c < 32 || c == 127; }
static int r_print(int c) { return c >= 32 && c < 127; }
static int r_graph(int c) { return c > 32 && c < 127; }

#define SWEEP(fn, ref) do { volatile int c; int i; for (i = 0; i < 128; i++) { c = i; \
    if ((fn(c, loc) != 0) != ref(i)) { tc_fail_ints(__FILE__, __LINE__, #fn, #ref, fn(c, loc) != 0, ref(i)); break; } } } while (0)

static int r_alnum(int c) { return r_alpha(c) || r_digit(c); }

TC_CASE(alpha_digit_alnum) { SWEEP(_isalpha_l, r_alpha); SWEEP(_isdigit_l, r_digit); SWEEP(_isalnum_l, r_alnum); }
TC_CASE(upper_lower) { SWEEP(_isupper_l, r_upper); SWEEP(_islower_l, r_lower); }
TC_CASE(space_cntrl) { SWEEP(_isspace_l, r_space); SWEEP(_iscntrl_l, r_cntrl); }
TC_CASE(xdigit) { SWEEP(_isxdigit_l, r_xdigit); }
TC_CASE(print_graph) { SWEEP(_isprint_l, r_print); SWEEP(_isgraph_l, r_graph); }

int main(void)
{
    tc_begin();
    p_create_locale = (tc_cl_t)tc_crt_proc("_create_locale"); p_free_locale = (tc_fl_t)tc_crt_proc("_free_locale");
    if (!p_create_locale || !p_free_locale) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    loc = p_create_locale(LC_ALL, v_c);
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create C locale\n"); return 1; }
    TC_RUN(alpha_digit_alnum); TC_RUN(upper_lower); TC_RUN(space_cntrl); TC_RUN(xdigit); TC_RUN(print_graph);
    p_free_locale(loc);
    return tc_end();
}
