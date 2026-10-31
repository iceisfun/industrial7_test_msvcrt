/* TC
 * id:    wcs_neg_wcstok_s_misuse
 * area:  wstring
 * funcs:
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s: context is ignored when str is non-NULL; NULL str with NULL *context is invalid (EINVAL)
 * note:  The "continue after exhaustion" behaviour is probed in wcs_neg_wcstok_s_exhausted_noiph_probe.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(wchar_t *, wcstok_s, (wchar_t *, const wchar_t *, wchar_t **));

TC_CASE(empty_string_returns_null_no_iph)
{
    wchar_t s[4] = { 0 }; wchar_t *ctx = NULL, *t;
    if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    t = tc_p_wcstok_s(s, L" ,", &ctx);
    CHECK(t == NULL); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(delims_only_returns_null_no_iph)
{
    wchar_t s[8] = { L' ', L',', L' ', L',', 0 }; wchar_t *ctx = NULL, *t;
    if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    t = tc_p_wcstok_s(s, L" ,", &ctx);
    CHECK(t == NULL); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nonnull_str_resets_context)
{
    wchar_t other[] = L"zz yy"; wchar_t s[] = L"ab cd"; wchar_t *ctx = other + 1, *t;
    if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    t = tc_p_wcstok_s(s, L" ", &ctx);
    CHECK(t == s); CHECK_WSTR(t, L"ab");
    t = tc_p_wcstok_s(NULL, L" ", &ctx);
    CHECK(t == s + 3); CHECK_WSTR(t, L"cd");
    CHECK_WSTR(other, L"zz yy");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_str_null_ctx_einval)
{
    wchar_t *ctx = NULL, *t;
    if (!tc_p_wcstok_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    t = tc_p_wcstok_s(NULL, L" ", &ctx);
    CHECK(t == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcstok_s);
    TC_RUN(empty_string_returns_null_no_iph); TC_RUN(delims_only_returns_null_no_iph);
    TC_RUN(nonnull_str_resets_context); TC_RUN(null_str_null_ctx_einval);
    return tc_end();
}
