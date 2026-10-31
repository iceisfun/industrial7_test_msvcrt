/* TC
 * id:    ucr_stdio_vsscanf_hexfloat_probe
 * area:  ucrt_only
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C99 7.21.6.2 (strtod subject sequence)
 * note:  PROBE: whether sscanf %lf accepts a C99 hex float (0x1.8p1 == 3.0). C99 requires strtod-style syntax; UCRT scanf support believed but unverified.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

static int vsc(unsigned long long opts, const char *in, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(hex) { double d = 0; CHECK_EQ_I(vsc(0, "0x1.8p1", "%lf", &d), 1); CHECK_BITS_D(d, 3.0); }

int main(void)
{
    tc_begin();
    TC_RUN(hex);
    return tc_end();
}
