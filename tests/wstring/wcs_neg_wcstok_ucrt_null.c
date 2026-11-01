/* TC
 * id:    wcs_neg_wcstok_ucrt_null
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strtok, wcstok (UCRT wcstok takes a context argument)
 * note:  PROBE: ucrtbase's exported wcstok is the 3-argument form and is expected to validate like wcstok_s:
 *        NULL return, errno EINVAL, invalid-parameter handler called.
 *        Observed Win10 22H2 ucrtbase: no validation. wcstok(str,NULL,&ctx) and wcstok(NULL,delim,&NULL-ctx) fault with 0xC0000005 (class C,
 *        asserted in child processes); wcstok(str,delim,NULL) falls back to per-thread state and returns "a" with errno 0 and no handler
 *        call. Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tk;

static void run_av(const char *n) { DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(null_delim)   /* class C: observed access violation, no validation */
{
    if (!tk) TC_SKIP("unavailable");
    run_av("nulldelim");
}
TC_CASE(null_context)   /* observed: NULL context falls back to per-thread state, no validation */
{
    wchar_t b[] = L"a b"; wchar_t *r;
    if (!tk || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0; errno = 0; r = tk(b, L" ", NULL);
    CHECK(r != NULL); if (r) CHECK_EQ_I(r[0], L'a'); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_str_null_ctx_value)   /* class C: observed access violation */
{
    if (!tk) TC_SKIP("unavailable");
    run_av("nullstr");
}
int main(void)
{
    const char *child;
    tc_begin(); tk = (wcstok3_t)tc_crt_proc("wcstok");
    if ((child = tc_child_name()) != NULL) {
        wchar_t b[] = L"a b"; wchar_t *ctx = NULL;
        if (!tk) return 78;
        tc_iph_install();
        if (tc_strequ(child, "nulldelim")) (void)tk(b, NULL, &ctx);
        else (void)tk(NULL, L" ", &ctx);
        return 0;   /* only reached when the call did not fault */
    }
    TC_RUN(null_delim); TC_RUN(null_context); TC_RUN(null_str_null_ctx_value);
    return tc_end();
}
