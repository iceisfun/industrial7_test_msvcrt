/* TC
 * id:    pfo_vsprintf_length_modifier_controls
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf Size specification (hh, h, l, ll, I64, j, z, t supported by UCRT)
 * note:  Positive controls for pfo_neg_probe_vsprintf_bad_length_modifiers: valid length modifiers succeed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int c0(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(i64)  { char b[32]; int r = c0(b, sizeof b, "%I64d", 5000000000LL); CHECK_EQ_I(r, 10); CHECK_STR(b, "5000000000"); }
TC_CASE(hh)   { char b[32]; int r = c0(b, sizeof b, "%hhd", 300); CHECK_EQ_I(r, 2); CHECK_STR(b, "44"); }
TC_CASE(z_j_t){ char b[32]; int r = c0(b, sizeof b, "%zd|%jd|%td", (size_t)1, (long long)-2, (ptrdiff_t)3); CHECK_EQ_I(r, 6); CHECK_STR(b, "1|-2|3"); }
TC_CASE(l_ll) { char b[32]; int r = c0(b, sizeof b, "%ld|%lld", 7L, 8LL); CHECK_EQ_I(r, 3); CHECK_STR(b, "7|8"); }
int main(void)
{
    tc_begin();
    TC_RUN(i64); TC_RUN(hh); TC_RUN(z_j_t); TC_RUN(l_ll);
    return tc_end();
}
