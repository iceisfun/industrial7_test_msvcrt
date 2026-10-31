/* TC
 * id:    pfo_neg_probe_vsprintf_s_zero_size
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf_s (sizeOfBuffer)
 * note:  PROBE: a non-NULL buffer with size 0 is rejected as an invalid parameter (handler once, return -1, EINVAL
 *        or ERANGE not asserted, buffer untouched). Not explicitly documented.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int calls(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(zero_size)
{
    char b[16]; volatile size_t n = 0; int r, i; for (i = 0; i < 16; i++) b[i] = 'Z'; tc_iph_count = 0;
    r = calls(0, b, n, "%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Z');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(zero_size);
    return tc_end();
}
