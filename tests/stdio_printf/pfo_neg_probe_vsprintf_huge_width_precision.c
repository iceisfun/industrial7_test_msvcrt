/* TC
 * id:    pfo_neg_probe_vsprintf_huge_width_precision
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf width/precision specification (no limit stated for overflowing values)
 * note:  PROBES: (1) width 2147483648 and precision 2147483648 (overflow int) make the call fail (negative return) and never overrun the buffer;
 *        (2) %*d with width INT_MIN does not crash or overrun (any return value accepted). Control %5d is certain.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

#define G 0x5A
static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int guard_ok(const char *b, int from, int to)
{ int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }
static const char * volatile fw = "%2147483648d";
static const char * volatile fp = "%.2147483648d";
static const char * volatile fs = "%*d";
static volatile int vi = 7;

TC_CASE(control) { char b[64]; int r; memset(b, G, sizeof b); r = cv(b, 32, "%5d", (int)vi); CHECK_EQ_I(r, 5); CHECK_STR(b, "    7"); }
TC_CASE(width_overflow) { char b[64]; int r; memset(b, G, sizeof b); r = cv(b, 32, fw, (int)vi); CHECK(r < 0); CHECK(guard_ok(b, 32, 64)); }
TC_CASE(precision_overflow) { char b[64]; int r; memset(b, G, sizeof b); r = cv(b, 32, fp, (int)vi); CHECK(r < 0); CHECK(guard_ok(b, 32, 64)); }
TC_CASE(int_min_width) { char b[64]; int r; memset(b, G, sizeof b); r = cv(b, 32, fs, INT_MIN, (int)vi); (void)r; CHECK(guard_ok(b, 32, 64)); }
int main(void)
{
    tc_begin();
    TC_RUN(control); TC_RUN(width_overflow); TC_RUN(precision_overflow); TC_RUN(int_min_width);
    return tc_end();
}
