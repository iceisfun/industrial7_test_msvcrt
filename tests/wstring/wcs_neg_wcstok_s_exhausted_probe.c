/* TC
 * id:    wcs_neg_wcstok_s_exhausted_probe
 * area:  wstring
 * funcs: 
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s, wcstok_s (repeat calls after exhaustion not documented)
 * note:  PROBES: after wcstok_s returned NULL at end of string, further wcstok_s(NULL,...) with the same context keep returning NULL (context points at the terminator, not NULL) and do not invoke the invalid-parameter handler.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(wchar_t *, wcstok_s, (wchar_t *, const wchar_t *, wchar_t **));
static const wchar_t *volatile pd = L",";
TC_CASE(repeat) { wchar_t b[] = L"a,b"; wchar_t *ctx = NULL, *t; if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    t = tc_p_wcstok_s(b, pd, &ctx); t = tc_p_wcstok_s(NULL, pd, &ctx);
    t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t == NULL);
    t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t == NULL);
    t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t == NULL);
    CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcstok_s); TC_RUN(repeat); return tc_end(); }
