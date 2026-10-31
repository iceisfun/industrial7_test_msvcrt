/* TC
 * id:    pfq_neg_stdio_common_vsprintf_p_optbits_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_p
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (options mask of __stdio_common_vsprintf_p is only partly documented in corecrt_stdio_config.h)
 * note:  PROBES that setting an unassigned high options bit (1<<40) does not crash and either succeeds ("7", r==1) or fails with -1.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(unknown_bit)
{
    char b[16]; int r; b[0] = 0;
    r = cs(1ULL << 40, b, sizeof b, "%1$d", 7);
    CHECK(r == 1 || r == -1);
    if (r == 1) CHECK_STR(b, "7");
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(unknown_bit);
    return tc_end();
}
