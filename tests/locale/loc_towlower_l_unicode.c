/* TC
 * id:    loc_towlower_l_unicode
 * area:  locale
 * funcs:
 * procs: _towlower_l _towupper_l _iswalpha_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs towlower, _towlower_l, iswalpha, _iswalpha_l
 * note:  In a non-C (1252) locale wide mapping/classification uses Unicode tables. Skipped if the locale cannot be created.
 */
#include "tc.h"
#include <wchar.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef unsigned int (__cdecl *wmap_t)(unsigned int, void *);
typedef int (__cdecl *wcls_t)(unsigned int, void *);
static create_t p_create; static free_t p_free; static wmap_t p_lower, p_upper; static wcls_t p_alpha;
static void *loc;
static volatile unsigned int c_E9up = 0x00C9, c_alpha = 0x03B1, c_e9 = 0x00E9, c_one = '1';

TC_CASE(latin_pair) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_U(p_lower(c_E9up, loc), 0x00E9); CHECK_EQ_U(p_upper(c_e9, loc), 0x00C9); }
TC_CASE(greek_pair) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_U(p_upper(c_alpha, loc), 0x0391); CHECK_EQ_U(p_lower(0x0391 + (c_one - c_one), loc), 0x03B1); }
TC_CASE(iswalpha_unicode) { if (!loc) TC_SKIP("no locale"); CHECK(p_alpha(c_e9, loc) != 0); }
TC_CASE(iswalpha_digit_false) { if (!loc) TC_SKIP("no locale"); CHECK_EQ_I(p_alpha(c_one, loc), 0); }

int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_lower = (wmap_t)tc_crt_proc("_towlower_l"); p_upper = (wmap_t)tc_crt_proc("_towupper_l"); p_alpha = (wcls_t)tc_crt_proc("_iswalpha_l");
    if (!p_create || !p_free || !p_lower || !p_upper || !p_alpha) { tc_fail_total++; tc_write("TC FAIL missing export\n"); return 1; }
    loc = p_create(0 /*LC_ALL*/, "English_United States.1252");
    TC_RUN(latin_pair); TC_RUN(greek_pair); TC_RUN(iswalpha_unicode); TC_RUN(iswalpha_digit_false);
    if (loc) p_free(loc);
    return tc_end();
}
