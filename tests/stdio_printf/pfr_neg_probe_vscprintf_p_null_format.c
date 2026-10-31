/* TC
 * id:    pfr_neg_probe_vscprintf_p_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vscprintf_p, _scwprintf_p, __stdio_common_vsprintf_p, __stdio_common_vswprintf_p
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _scprintf_p, _vscprintf_p, _scwprintf_p, _vscwprintf_p (NULL format is an invalid parameter: -1, EINVAL)
 * note:  PROBE (unknown-conversion case only on ucrtbase; pre-UCRT parsers may treat "%y" as literal text): the positional length-counting functions reject NULL / malformed formats: -1, errno EINVAL, handler called
 *        at least once (the exact count is not asserted: wrappers may validate twice). Only _scprintf_p is covered elsewhere. On ucrtbase the functions are inline wrappers over
 *        __stdio_common_vsprintf_p / __stdio_common_vswprintf_p (options 2, NULL buffer, count 0), which are called directly.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

typedef int (__cdecl *vscp_t)(const char *, va_list);
typedef int (__cdecl *scwp_t)(const wchar_t *, ...);
static vscp_t p_vscp; static scwp_t p_scwp;
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsp_p_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vswp_p_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsp_p_t p_vsp_p; static vswp_p_t p_vswp_p;
static int ucrt_vscp(const char *f, va_list ap) { return p_vsp_p(2, NULL, 0, f, NULL, ap); }
static int ucrt_scwp(const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vswp_p(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }
#endif
static int vc(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vscp(f, ap); va_end(ap); return r; }

TC_CASE(vscprintf_p_null)
{
    const char *volatile f = NULL; int r;
    if (!p_vscp) TC_SKIP("no _vscprintf_p");
    tc_iph_count = 0; errno = 0;
    r = vc(f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(scwprintf_p_null)
{
    const wchar_t *volatile f = NULL; int r;
    if (!p_scwp) TC_SKIP("no _scwprintf_p");
    tc_iph_count = 0; errno = 0;
    r = p_scwp(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(vscprintf_p_unknown_conv)
{
    const char *volatile f = "%y"; int r;
    if (!p_vscp) TC_SKIP("no _vscprintf_p");
    if (TC_BELOW(140)) TC_SKIP("old CRTs may print an unknown conversion literally; only probed on ucrtbase");
    tc_iph_count = 0; errno = 0;
    r = vc(f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(vscprintf_p_valid)
{
    const char *volatile f = "%2$s%1$s"; int r;
    if (!p_vscp) TC_SKIP("no _vscprintf_p");
    tc_iph_count = 0;
    r = vc(f, "ab", "cde");
    CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
#if TC_CRT_VER >= 140
    p_vsp_p = (vsp_p_t)tc_crt_proc("__stdio_common_vsprintf_p");
    p_vswp_p = (vswp_p_t)tc_crt_proc("__stdio_common_vswprintf_p");
    if (p_vsp_p) p_vscp = ucrt_vscp;
    if (p_vswp_p) p_scwp = ucrt_scwp;
#else
    p_vscp = (vscp_t)tc_crt_proc("_vscprintf_p");
    p_scwp = (scwp_t)tc_crt_proc("_scwprintf_p");
#endif
    TC_RUN(vscprintf_p_null); TC_RUN(scwprintf_p_null); TC_RUN(vscprintf_p_unknown_conv); TC_RUN(vscprintf_p_valid);
    return tc_end();
}
