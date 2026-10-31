/* TC
 * id:    pfo_neg_probe_vsprintf_p_percent_n_positional
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_p
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_p (positional parameters); MS docs printf (%n disabled by default)
 * note:  PROBES: whether the _p (positional) entry rejects %n like the other printf entries (-1, EINVAL, target not written).
 *        The printf_p docs do not state this, so it is kept apart from the documented %n tests.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define SENT 0x5a5a5a5a
static int cp(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(positional_p_rejects_n)
{
    char b[16]; int t = SENT, r;
    tc_iph_count = 0; errno = 0;
    r = cp(b, sizeof b, "ab%1$n", &t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t, SENT); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(positional_p_rejects_n);
    return tc_end();
}
