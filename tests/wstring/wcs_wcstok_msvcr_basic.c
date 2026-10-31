/* TC
 * id:    wcs_wcstok_msvcr_basic
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcstok (pre-VS2015: wcstok(strToken, strDelimit), state kept per thread)
 * note:  Called through a 2-arg typedef; header may declare 3 args.
 */
#include "tc.h"
#include <wchar.h>
typedef wchar_t *(__cdecl *wcstok2_t)(wchar_t *, const wchar_t *);
static wcstok2_t tok;
TC_CASE(tokens) { wchar_t b[] = L"a b c"; wchar_t *t;
    t = tok(b, L" "); CHECK_WSTR(t, L"a"); CHECK_EQ_P(t, b); t = tok(NULL, L" "); CHECK_WSTR(t, L"b"); t = tok(NULL, L" "); CHECK_WSTR(t, L"c"); }
TC_CASE(null_cont) { wchar_t b[] = L"x;y"; CHECK_WSTR(tok(b, L";"), L"x"); CHECK_WSTR(tok(NULL, L";"), L"y"); }
TC_CASE(leading) { wchar_t b[] = L"  ,, a,b"; CHECK_WSTR(tok(b, L" ,"), L"a"); CHECK_WSTR(tok(NULL, L" ,"), L"b"); }
TC_CASE(end) { wchar_t b[] = L"a"; CHECK_WSTR(tok(b, L","), L"a"); CHECK(tok(NULL, L",") == NULL); CHECK(tok(NULL, L",") == NULL); }
TC_CASE(only_delims) { wchar_t b[] = L",,,"; CHECK(tok(b, L",") == NULL); }
int main(void) { tc_begin(); tok = (wcstok2_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    TC_RUN(tokens); TC_RUN(null_cont); TC_RUN(leading); TC_RUN(end); TC_RUN(only_delims); return tc_end(); }
