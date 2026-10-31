/* TC
 * id:    pfq_neg_probe_vscprintf_p_errors
 * area:  stdio_printf
 * funcs: _scprintf_p _scwprintf_p
 * procs: _scprintf_p_l
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _scprintf_p, _scprintf_p_l, _scwprintf_p (NULL format -> invalid parameter handler, -1)
 * note:  PROBE: count-only positional functions fail with -1 and the invalid-parameter handler on a NULL wide format and on a gapped positional list. _scprintf_p_l (an export on msvcr90..120 only; an SDK inline on ucrtbase) with a NULL locale is the valid control.
 */
#include "tc.h"
#include "pfq_shim.h"

static const wchar_t * volatile Nw = NULL;
static const char * volatile Fgap = "%1$d%3$d", * volatile Fok = "%2$s-%1$d";
static const wchar_t * volatile Wgap = L"%1$d%3$d";

#ifndef _UCRT
typedef int (__cdecl *scp_l_t)(const char *, void *, ...);
#endif

TC_CASE(scw_null_fmt)
{
    int r; tc_iph_count = 0;
    r = pq_scwprintf_p(Nw);
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
}
TC_CASE(sc_gap)
{
    int r; tc_iph_count = 0;
    r = pq_scprintf_p(Fgap, 1, 2, 3);
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
}
TC_CASE(scw_gap)
{
    int r; tc_iph_count = 0;
    r = pq_scwprintf_p(Wgap, 1, 2, 3);
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
}
TC_CASE(sc_l_null_locale_control)
{
    int r; tc_iph_count = 0;
#ifdef _UCRT
    r = pq_scprintf_p(Fok, 42, "ab");  /* ucrtbase: _scprintf_p_l is inline over the same call with a NULL locale */
    CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0);
#else
    scp_l_t p = (scp_l_t)tc_crt_proc("_scprintf_p_l");
    if (!p) TC_SKIP("_scprintf_p_l not exported");
    r = p(Fok, NULL, 42, "ab");
    CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0);
#endif
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(scw_null_fmt); TC_RUN(sc_gap); TC_RUN(scw_gap); TC_RUN(sc_l_null_locale_control);
    return tc_end();
}
