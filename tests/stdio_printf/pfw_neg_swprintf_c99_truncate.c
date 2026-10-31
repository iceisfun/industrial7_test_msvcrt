/* TC
 * id:    pfw_neg_swprintf_c99_truncate
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf: negative if an encoding error occurred or n or more wide chars were requested; at most n wide chars written; MS docs swprintf (count form)
 * note:  Only the return value and the no-overrun canary are asserted; buffer NUL state after failure is not.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile vfmt = L"%d";
static const wchar_t * volatile vabcd = L"abcd";

TC_CASE(control_fits) { wchar_t b[16]; int n; fill(b); n = swprintf(b, 4, vfmt, 123); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123"); CHECK(b[4] == L'Z'); }
TC_CASE(ret_neg1_count4) { wchar_t b[16]; int n; fill(b); n = swprintf(b, 4, vfmt, 1234567); CHECK(n < 0); CHECK(b[4] == L'Z'); }
TC_CASE(exact_fit_len_eq_count_ret_neg1) { wchar_t b[16]; int n; fill(b); n = swprintf(b, 4, L"%ls", vabcd); CHECK(n < 0); }
TC_CASE(no_overrun_canary)
{
    wchar_t b[16]; int n, i; fill(b); n = swprintf(b, 4, vfmt, 1234567); CHECK(n < 0);
    for (i = 4; i < 16; i++) CHECK(b[i] == L'Z');
    fill(b); n = swprintf(b, 4, L"%ls", vabcd); CHECK(n < 0);
    for (i = 4; i < 16; i++) CHECK(b[i] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(control_fits); TC_RUN(ret_neg1_count4); TC_RUN(exact_fit_len_eq_count_ret_neg1); TC_RUN(no_overrun_canary); return tc_end(); }
