/* TC
 * id:    loc_probe_ctype_l_print_cntrl_high_bytes_c
 * area:  locale
 * funcs: _isprint_l _iscntrl_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setlocale (C locale); C11 7.4.1 (printing set is implementation-defined)
 * note:  PROBE: in the C locale bytes 0x80..0xFF are neither printable nor control (MS C-locale ctype table is zero above 0x7F). C11 leaves the printing set implementation-defined, so this is not asserted as standard.
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

#define SWEEP_HI(expr) do { int i; for (i = 0x80; i <= 0xFF; i++) { volatile int c = i; if (!(expr)) { tc_fail_ints(__FILE__, __LINE__, #expr, "true", i, 0); break; } } } while (0)

TC_CASE(not_print_not_cntrl) { SWEEP_HI(_isprint_l(c, loc) == 0); SWEEP_HI(_iscntrl_l(c, loc) == 0); }

int main(void)
{
    tc_begin();
    p_create_locale = (tc_cl_t)tc_crt_proc("_create_locale"); p_free_locale = (tc_fl_t)tc_crt_proc("_free_locale");
    if (!p_create_locale || !p_free_locale) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    loc = p_create_locale(LC_ALL, v_c);
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create C locale\n"); return 1; }
    TC_RUN(not_print_not_cntrl);
    p_free_locale(loc);
    return tc_end();
}
