/* TC
 * id:    wcs_wcstok_ucrt_basic
 * area:  wstring
 * funcs:
 * procs: wcstok
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strtok, _strtok_l, wcstok, _wcstok_l (VS2015+: wcstok takes context); C11 7.29.4.5.8
 * note:  ucrtbase wcstok is the 3-argument standard form
 */
#include "tc.h"
#include <wchar.h>
typedef wchar_t *(__cdecl *wcstok3_t)(wchar_t *, const wchar_t *, wchar_t **);
static wcstok3_t tok;
TC_CASE(tokens) { wchar_t b[] = L"a b c"; wchar_t *c = NULL, *t;
    t = tok(b, L" ", &c); CHECK_WSTR(t, L"a"); t = tok(NULL, L" ", &c); CHECK_WSTR(t, L"b");
    t = tok(NULL, L" ", &c); CHECK_WSTR(t, L"c"); t = tok(NULL, L" ", &c); CHECK(t == NULL); }
TC_CASE(leading) { wchar_t b[] = L"  ,, a,b"; wchar_t *c = NULL, *t;
    t = tok(b, L" ,", &c); CHECK_WSTR(t, L"a"); t = tok(NULL, L" ,", &c); CHECK_WSTR(t, L"b"); t = tok(NULL, L" ,", &c); CHECK(t == NULL); }
TC_CASE(collapse) { wchar_t b[] = L"a,,,b"; wchar_t *c = NULL, *t;
    t = tok(b, L",", &c); CHECK_WSTR(t, L"a"); t = tok(NULL, L",", &c); CHECK_WSTR(t, L"b"); t = tok(NULL, L",", &c); CHECK(t == NULL); }
TC_CASE(empty) { wchar_t b[] = L""; wchar_t *c = NULL; CHECK(tok(b, L",", &c) == NULL); }
TC_CASE(all_delims) { wchar_t b[] = L",,,"; wchar_t *c = NULL; CHECK(tok(b, L",", &c) == NULL); }
TC_CASE(in_place) { wchar_t b[] = L"ab,cd"; wchar_t *c = NULL, *t = tok(b, L",", &c);
    CHECK_EQ_P(t, b); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], L'c'); CHECK_EQ_I(b[4], L'd'); }
TC_CASE(interleaved) { wchar_t b1[] = L"a b c", b2[] = L"x y z"; wchar_t *c1 = NULL, *c2 = NULL;
    CHECK_WSTR(tok(b1, L" ", &c1), L"a"); CHECK_WSTR(tok(b2, L" ", &c2), L"x");
    CHECK_WSTR(tok(NULL, L" ", &c1), L"b"); CHECK_WSTR(tok(NULL, L" ", &c2), L"y");
    CHECK_WSTR(tok(NULL, L" ", &c1), L"c"); CHECK_WSTR(tok(NULL, L" ", &c2), L"z");
    CHECK(tok(NULL, L" ", &c1) == NULL); CHECK(tok(NULL, L" ", &c2) == NULL); }
int main(void) { tc_begin(); tok = (wcstok3_t)tc_crt_proc("wcstok"); CHECK(tok != NULL); if (!tok) return tc_end();
    TC_RUN(tokens); TC_RUN(leading); TC_RUN(collapse); TC_RUN(empty); TC_RUN(all_delims); TC_RUN(in_place); TC_RUN(interleaved); return tc_end(); }
