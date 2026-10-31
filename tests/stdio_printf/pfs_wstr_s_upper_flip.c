/* TC
 * id:    pfs_wstr_s_upper_flip
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field characters (wprintf: S/hs = narrow)

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile abc = "abc";
static const char *volatile abcd = "abcd";
static const wchar_t *volatile wl = L"w";

TC_CASE(S)  { wchar_t w[32]; int n = _snwprintf(w, 32, L"%S", abc); CHECK_WSTR(w, L"abc"); CHECK_EQ_I(n, 3); }
TC_CASE(hs) { wchar_t w[32]; _snwprintf(w, 32, L"%hs", abc); CHECK_WSTR(w, L"abc"); }
TC_CASE(hS) { wchar_t w[32]; _snwprintf(w, 32, L"%hS", abc); CHECK_WSTR(w, L"abc"); }
TC_CASE(width) { wchar_t w[32]; _snwprintf(w, 32, L"[%5hs][%-5hs]", abc, abc); CHECK_WSTR(w, L"[  abc][abc  ]"); }
TC_CASE(prec) { wchar_t w[32]; _snwprintf(w, 32, L"[%.2hs]", abcd); CHECK_WSTR(w, L"[ab]"); }
TC_CASE(mixed) { wchar_t w[32]; int n = _snwprintf(w, 32, L"%s %hs %S %ls", wl, abc, abc, wl); CHECK_WSTR(w, L"w abc abc w"); CHECK_EQ_I(n, 11); }

int main(void) { tc_begin(); TC_RUN(S); TC_RUN(hs); TC_RUN(hS); TC_RUN(width); TC_RUN(prec); TC_RUN(mixed); return tc_end(); }
