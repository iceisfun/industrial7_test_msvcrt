/* TC
 * id:    ct_neg_probe_create_locale_bad_category
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale (category LC_ALL..LC_TIME)
 * note:  PROBE: out-of-range categories (-1, 100) are rejected with NULL and do not crash. IPH count not asserted.
 */
#include "tc.h"

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
static cl_t p_cl; static fl_t p_fl;

TC_CASE(neg1) { volatile int cat = -1; void *l = p_cl(cat, "C"); CHECK(l == NULL); if (l) p_fl(l); }
TC_CASE(cat100) { volatile int cat = 100; void *l = p_cl(cat, "C"); CHECK(l == NULL); if (l) p_fl(l); }
int main(void)
{
    tc_begin(); tc_iph_install();
    p_cl = (cl_t)tc_crt_proc("_create_locale"); p_fl = (fl_t)tc_crt_proc("_free_locale");
    if (!p_cl || !p_fl) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(neg1); TC_RUN(cat100);
    return tc_end();
}
