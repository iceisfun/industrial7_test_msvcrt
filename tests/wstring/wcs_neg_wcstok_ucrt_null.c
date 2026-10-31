/* TC
 * id:    wcs_neg_wcstok_ucrt_null
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok, wcstok (UCRT wcstok takes a context argument)
 * note:  PROBE: ucrtbase's exported wcstok is the 3-argument form and is expected to validate like wcstok_s:
 *        NULL return, errno EINVAL, invalid-parameter handler called.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tk;

TC_CASE(null_delim)
{
    wchar_t b[] = L"a b"; wchar_t *ctx = NULL, *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0; r = tk(b, NULL, &ctx);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_context)
{
    wchar_t b[] = L"a b"; wchar_t *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0; r = tk(b, L" ", NULL);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_str_null_ctx_value)
{
    wchar_t *ctx = NULL, *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0; r = tk(NULL, L" ", &ctx);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tk = (wcstok3_t)tc_crt_proc("wcstok");
    TC_RUN(null_delim); TC_RUN(null_context); TC_RUN(null_str_null_ctx_value);
    return tc_end();
}
