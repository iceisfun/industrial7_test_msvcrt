/* TC
 * id:    pfs_wstr_s_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters (wprintf: s = wide)
 * note:  _snwprintf used so the same import is valid on all CRTs
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile wabc = L"abc";
static const wchar_t *volatile wabcd = L"abcd";
static volatile int three = 3, neg = -2;

TC_CASE(s)  { wchar_t w[32]; int n = _snwprintf(w, 32, L"%s", wabc); CHECK_WSTR(w, L"abc"); CHECK_EQ_I(n, 3); }
TC_CASE(ls_ws) { wchar_t w[32]; _snwprintf(w, 32, L"%ls|%ws", wabc, wabc); CHECK_WSTR(w, L"abc|abc"); }
TC_CASE(width) { wchar_t w[32]; int n = _snwprintf(w, 32, L"[%6s][%-6s]", wabc, wabc); CHECK_WSTR(w, L"[   abc][abc   ]"); CHECK_EQ_I(n, 16); }
TC_CASE(prec) { wchar_t w[32]; _snwprintf(w, 32, L"[%.2s]", wabcd); CHECK_WSTR(w, L"[ab]"); }
TC_CASE(star) { wchar_t w[32]; _snwprintf(w, 32, L"[%.*s]", three, wabcd); CHECK_WSTR(w, L"[abc]"); }
TC_CASE(neg_star) { wchar_t w[32]; _snwprintf(w, 32, L"[%.*s]", neg, wabc); CHECK_WSTR(w, L"[abc]"); }

int main(void) { tc_begin(); TC_RUN(s); TC_RUN(ls_ws); TC_RUN(width); TC_RUN(prec); TC_RUN(star); TC_RUN(neg_star); return tc_end(); }
