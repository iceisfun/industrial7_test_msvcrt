/* TC
 * id:    ct_neg_probe_create_locale_null_name
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale
 * note:  PROBE: _create_locale(LC_ALL, NULL) returns NULL; ucrt sets errno=EINVAL and calls the IPH once. The
 *        errno/IPH checks are in their own cases and only run on ucrtbase.
 */
#include "tc.h"
#include <errno.h>

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
static cl_t p_cl; static fl_t p_fl;

TC_CASE(returns_null) { volatile const char *nm = NULL; void *l = p_cl(0, (const char *)nm); CHECK(l == NULL); if (l) p_fl(l); }
TC_CASE(errno_einval)
{
#ifdef TC_UCRT
    volatile const char *nm = NULL; void *l; errno = 0;
    l = p_cl(0, (const char *)nm); CHECK(l == NULL); CHECK_EQ_I(errno, EINVAL); if (l) p_fl(l);
#else
    TC_SKIP("ucrtbase only");
#endif
}
TC_CASE(iph_once)
{
#ifdef TC_UCRT
    volatile const char *nm = NULL; void *l; int n = tc_iph_count;
    l = p_cl(0, (const char *)nm); CHECK_EQ_I(tc_iph_count - n, 1); if (l) p_fl(l);
#else
    TC_SKIP("ucrtbase only");
#endif
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_cl = (cl_t)tc_crt_proc("_create_locale"); p_fl = (fl_t)tc_crt_proc("_free_locale");
    if (!p_cl || !p_fl) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(returns_null); TC_RUN(errno_einval); TC_RUN(iph_once);
    return tc_end();
}
