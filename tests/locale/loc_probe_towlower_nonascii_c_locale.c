/* TC
 * id:    loc_probe_towlower_nonascii_c_locale
 * area:  locale
 * funcs:
 * procs: _towlower_l _towupper_l _iswalpha_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs towlower, _towlower_l
 * note:  PROBE: in a "C" _locale_t case mapping is ASCII-only (0xC9 stays 0xC9, 0xE9 stays 0xE9) while
 *        _iswalpha_l(0xE9) is nonzero (wide ctype table covers Latin-1). msvcr90 may differ.
 */
#include "tc.h"
#include <wchar.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef unsigned int (__cdecl *wmap_t)(unsigned int, void *);
typedef int (__cdecl *wcls_t)(unsigned int, void *);
static create_t p_create; static free_t p_free; static wmap_t p_lower, p_upper; static wcls_t p_alpha;
static void *loc;
static volatile unsigned int c_C9 = 0x00C9, c_e9 = 0x00E9;

TC_CASE(towlower_nonascii_identity) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_U(p_lower(c_C9, loc), 0x00C9); }
TC_CASE(iswalpha_nonascii_true_in_C) { if (!loc) TC_SKIP("no locale"); CHECK(p_alpha(c_e9, loc) != 0); }
TC_CASE(towupper_nonascii_identity) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_U(p_upper(c_e9, loc), 0x00E9); }

int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_lower = (wmap_t)tc_crt_proc("_towlower_l"); p_upper = (wmap_t)tc_crt_proc("_towupper_l"); p_alpha = (wcls_t)tc_crt_proc("_iswalpha_l");
    if (!p_create || !p_free || !p_lower || !p_upper || !p_alpha) { tc_fail_total++; tc_write("TC FAIL missing export\n"); return 1; }
    loc = p_create(0, "C");
    TC_RUN(towlower_nonascii_identity); TC_RUN(iswalpha_nonascii_true_in_C); TC_RUN(towupper_nonascii_identity);
    if (loc) p_free(loc);
    return tc_end();
}
