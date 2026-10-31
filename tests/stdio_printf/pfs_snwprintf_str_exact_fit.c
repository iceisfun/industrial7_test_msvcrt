/* TC
 * id:    pfs_snwprintf_str_exact_fit
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snprintf_l, _snwprintf, _snwprintf_l
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const char *vn = "abcdef";
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }

TC_CASE(room) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 8, L"%s", L"abc"); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); }
TC_CASE(exact) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 3, L"%s", L"abc"); CHECK_EQ_I(n, 3); CHECK(b[0]==L'a' && b[2]==L'c'); CHECK_EQ_I(b[3], L'Z'); }
TC_CASE(S_narrow) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 16, L"%S", (const char *)vn); CHECK_EQ_I(n, 6); CHECK_WSTR(b, L"abcdef"); }
TC_CASE(prec_trunc) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 16, L"%.3s", L"abcdef"); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); }
int main(void) { tc_begin(); TC_RUN(room); TC_RUN(exact); TC_RUN(S_narrow); TC_RUN(prec_trunc); return tc_end(); }
