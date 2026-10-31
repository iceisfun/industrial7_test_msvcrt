/* TC
 * id:    wcs_neg_wcstok_null_context
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s, _strtok_s_l, wcstok_s (validation documented for the _s forms only)
 * note:  PROBE: the docs for wcstok do not state parameter validation; asserting that ucrtbase wcstok(str,NULL,&ctx) and
 *        wcstok(NULL,delim,&NULL-ctx) behave like wcstok_s (return NULL, EINVAL, invalid-parameter handler once).
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tok;
TC_CASE(null_delim) { wchar_t b[] = L"a b"; wchar_t *c = NULL, *r; tc_iph_count = 0; errno = 0;
    r = tok(b, NULL, &c); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_start_null_ctx) { wchar_t *c = NULL, *r; tc_iph_count = 0; errno = 0;
    r = tok(NULL, L" ", &c); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); tok = (wcstok3_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    CHECK(tc_iph_install()); TC_RUN(null_delim); TC_RUN(null_start_null_ctx); return tc_end(); }
