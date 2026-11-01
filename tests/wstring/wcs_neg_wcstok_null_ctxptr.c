/* TC
 * id:    wcs_neg_wcstok_null_ctxptr
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strtok_s (context must not be NULL); wcstok in ucrt takes a context argument
 * note:  PROBE: wcstok(str,delim,NULL) - does ucrt reject it (NULL, EINVAL, handler) or fall back to per-thread state?
 *        Observed Win10 22H2 ucrtbase: NO rejection: a NULL context falls back to per-thread state (first call returns "a" of "a b",
 *        a second wcstok(NULL,delim,NULL) returns "b"), errno stays 0, no handler call; deviates from wcstok_s validation. Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tok;
TC_CASE(null_context) { wchar_t b[] = L"a b"; wchar_t *r; tc_iph_count = 0; errno = 0;
    r = tok(b, L" ", NULL); CHECK(r != NULL); if (r) CHECK_EQ_I(r[0], L'a'); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);   /* observed */
    r = tok(NULL, L" ", NULL); CHECK(r != NULL); if (r) CHECK_EQ_I(r[0], L'b');   /* observed: per-thread state */ }
int main(void) { tc_begin(); tok = (wcstok3_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    CHECK(tc_iph_install()); TC_RUN(null_context); return tc_end(); }
