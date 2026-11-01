/* TC
 * id:    wcs_neg_wcstok_null_context
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strtok_s, _strtok_s_l, wcstok_s (validation documented for the _s forms only)
 * note:  PROBE: the docs for wcstok do not state parameter validation; asserting that ucrtbase wcstok(str,NULL,&ctx) and
 *        wcstok(NULL,delim,&NULL-ctx) behave like wcstok_s (return NULL, EINVAL, invalid-parameter handler once).
 *        Observed Win10 22H2 ucrtbase: the exported wcstok does NOT validate: wcstok(str,NULL,&ctx) and wcstok(NULL,delim,&NULL-ctx) both
 *        fault with 0xC0000005 (installed or default handler) -- class C: each runs in a child process and the fault is asserted. Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tok;
static void run_av(const char *n) { DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(null_delim_access_violation) { run_av("nulldelim"); }          /* observed: no validation */
TC_CASE(null_start_null_ctx_access_violation) { run_av("nullstart"); }  /* observed: no validation */
int main(void)
{
    const char *child;
    tc_begin(); tok = (wcstok3_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    CHECK(tc_iph_install());
    if ((child = tc_child_name()) != NULL) {
        wchar_t b[] = L"a b"; wchar_t *c = NULL;
        if (tc_strequ(child, "nulldelim")) (void)tok(b, NULL, &c);
        else (void)tok(NULL, L" ", &c);
        return 0;   /* only reached when the call did not fault */
    }
    TC_RUN(null_delim_access_violation); TC_RUN(null_start_null_ctx_access_violation); return tc_end();
}
