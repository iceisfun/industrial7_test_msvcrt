/* TC
 * id:    wcs_neg_wcstok_s_null
 * area:  wstring
 * funcs:
 * procs: wcstok_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtok_s, wcstok_s (parameter validation: EINVAL, invalid parameter handler)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

typedef wchar_t *(__cdecl *wcstok_s_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok_s_t tk;

TC_CASE(null_delim)
{
    wchar_t b[] = L"a b"; wchar_t *ctx = NULL, *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0;
    r = tk(b, NULL, &ctx);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_context)
{
    wchar_t b[] = L"a b"; wchar_t *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0;
    r = tk(b, L" ", NULL);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_str_null_ctx_value)
{
    wchar_t *ctx = NULL, *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0;
    r = tk(NULL, L" ", &ctx);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tk = (wcstok_s_t)tc_crt_proc("wcstok_s");
    TC_RUN(null_delim); TC_RUN(null_context); TC_RUN(null_str_null_ctx_value);
    return tc_end();
}
