/* TC
 * id:    wcs_neg_wcstok_s_exhausted_noiph_probe
 * area:  wstring
 * funcs:
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcstok_s
 * note:  Probes: after the last token has been returned, a further call with NULL str and the same context returns
 *        NULL and does NOT call the invalid-parameter handler (context is non-NULL, pointing at the final NUL).
 */
#include "tc.h"
#include <wchar.h>

TC_DECL_PROC(wchar_t *, wcstok_s, (wchar_t *, const wchar_t *, wchar_t **));

TC_CASE(null_str_after_exhaustion_no_iph)
{
    wchar_t s[] = L"one two"; wchar_t *ctx = NULL, *t;
    if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    t = tc_p_wcstok_s(s, L" ", &ctx); CHECK_WSTR(t, L"one");
    t = tc_p_wcstok_s(NULL, L" ", &ctx); CHECK_WSTR(t, L"two");
    t = tc_p_wcstok_s(NULL, L" ", &ctx); CHECK(t == NULL);
    t = tc_p_wcstok_s(NULL, L" ", &ctx); CHECK(t == NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(wcstok_s); TC_RUN(null_str_after_exhaustion_no_iph); return tc_end(); }
