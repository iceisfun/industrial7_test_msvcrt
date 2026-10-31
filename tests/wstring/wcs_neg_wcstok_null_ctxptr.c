/* TC
 * id:    wcs_neg_wcstok_null_ctxptr
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtok_s (context must not be NULL); wcstok in ucrt takes a context argument
 * note:  PROBE: wcstok(str,delim,NULL) - does ucrt reject it (NULL, EINVAL, handler) or fall back to per-thread state?
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tok;
TC_CASE(null_context) { wchar_t b[] = L"a b"; wchar_t *r; tc_iph_count = 0; errno = 0;
    r = tok(b, L" ", NULL); CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); tok = (wcstok3_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    CHECK(tc_iph_install()); TC_RUN(null_context); return tc_end(); }
