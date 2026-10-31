/* TC
 * id:    pfo_neg_probe_vsnprintf_null_count0_win7
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _vsnprintf
 * note:  PROBE: _vsnprintf(NULL, 0, "%d", ...) returns a negative value (the output cannot fit in 0 chars on the
 *        legacy _vsnprintf, which does not report a required length), and does not crash.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int call(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(null_count0)
{
    int r;
    tc_iph_count = 0;
    r = call(NULL, 0, "%d", 12345);
    CHECK(r < 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_count0);
    return tc_end();
}
