/* TC
 * id:    wcs_neg_wcstok_s_nodelim_state
 * area:  wstring
 * funcs: 
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s
 * note:  first NULL return at exhaustion, and restart with a new string; repeated calls after exhaustion are in wcs_neg_wcstok_s_exhausted_probe
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(wchar_t *, wcstok_s, (wchar_t *, const wchar_t *, wchar_t **));
static const wchar_t *volatile pd = L",";
TC_CASE(after_end) { wchar_t b[] = L"a,b"; wchar_t *ctx = NULL, *t; if (!tc_p_wcstok_s) TC_SKIP("missing");
    t = tc_p_wcstok_s(b, pd, &ctx); CHECK(t != NULL); CHECK_WSTR(t, L"a");
    t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t != NULL); CHECK_WSTR(t, L"b");
    t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t == NULL); }
TC_CASE(fresh) { wchar_t b[] = L"a,b"; wchar_t c[] = L"x,y"; wchar_t *ctx = NULL, *t; if (!tc_p_wcstok_s) TC_SKIP("missing");
    t = tc_p_wcstok_s(b, pd, &ctx); (void)t; t = tc_p_wcstok_s(NULL, pd, &ctx); t = tc_p_wcstok_s(NULL, pd, &ctx); CHECK(t == NULL);
    t = tc_p_wcstok_s(c, pd, &ctx); CHECK(t != NULL); CHECK_WSTR(t, L"x"); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcstok_s); TC_RUN(after_end); TC_RUN(fresh); return tc_end(); }
