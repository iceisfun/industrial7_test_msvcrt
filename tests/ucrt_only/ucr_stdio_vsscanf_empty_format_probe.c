/* TC
 * id:    ucr_stdio_vsscanf_empty_format_probe
 * area:  ucrt_only
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 paragraph 16
 * note:  PROBE: sscanf("abc", "") returns 0 (no conversion attempted, so not EOF per C11 7.21.6.2p16). Whether UCRT returns 0 vs EOF is unverified.
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

TC_CASE(empty_fmt) { int r = vsc(0, "abc", ""); CHECK_EQ_I(r, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(empty_fmt);
    return tc_end();
}
