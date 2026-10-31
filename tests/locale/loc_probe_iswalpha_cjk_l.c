/* TC
 * id:    loc_probe_iswalpha_cjk_l
 * area:  locale
 * funcs:
 * procs: _iswalpha_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs towlower, _towlower_l, iswalpha, _iswalpha_l
 * note:  PROBE: _iswalpha_l(U+4E2D) is nonzero in a 1252 locale (GetStringTypeW reports C1_ALPHA for CJK ideographs). Skipped if locale cannot be created.
 */
#include "tc.h"
#include <wchar.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *wcls_t)(unsigned int, void *);
static create_t p_create; static free_t p_free; static wcls_t p_alpha;
static void *loc;
static volatile unsigned int c_cjk = 0x4E2D;

TC_CASE(cjk_alpha) { if (!loc) TC_SKIP("no locale"); CHECK(p_alpha(c_cjk, loc) != 0); }

int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_alpha = (wcls_t)tc_crt_proc("_iswalpha_l");
    if (!p_create || !p_free || !p_alpha) { tc_fail_total++; tc_write("TC FAIL missing export\n"); return 1; }
    loc = p_create(0 /*LC_ALL*/, "English_United States.1252");
    TC_RUN(cjk_alpha);
    if (loc) p_free(loc);
    return tc_end();
}
