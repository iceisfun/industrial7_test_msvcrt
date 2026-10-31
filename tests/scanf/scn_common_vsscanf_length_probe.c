/* TC
 * id:    scn_common_vsscanf_length_probe
 * area:  scanf
 * funcs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snscanf ("count: number of characters to read"); stdio.h __stdio_common_vsscanf length parameter
 * note:  PROBE: the explicit length argument limits how many input characters are available, same machinery as _snscanf.
 *        Expected: "12345" with length 3 and "%d" yields 123; length 0 yields EOF (no input available).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cl(const char *in, size_t len, const char *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
    r = __stdio_common_vsscanf(0, in, len, fmt, NULL, ap);
    va_end(ap); return r;
}
static const char *volatile vin;
TC_CASE(len3)   { int a = 0, n; vin = "12345"; n = cl((const char *)vin, 3, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 123); }
TC_CASE(len0)   { int a = 8, n; vin = "12345"; n = cl((const char *)vin, 0, "%d", &a); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 8); }
TC_CASE(len_all) { int a = 0, n; vin = "12345"; n = cl((const char *)vin, 5, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 12345); }
TC_CASE(len_str) { char b[8]; int n; vin = "abcdef"; n = cl((const char *)vin, 2, "%s", b); CHECK_EQ_I(n, 1); CHECK_STR(b, "ab"); }
int main(void)
{
    tc_begin();
    TC_RUN(len3); TC_RUN(len0); TC_RUN(len_all); TC_RUN(len_str);
    return tc_end();
}
