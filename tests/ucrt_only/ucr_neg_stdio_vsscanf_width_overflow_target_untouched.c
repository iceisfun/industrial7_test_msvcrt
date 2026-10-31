/* TC
 * id:    ucr_neg_stdio_vsscanf_width_overflow_target_untouched
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2 (matching failure; out-of-range value is UB); MS docs scanf
 * note:  PROBES incomplete numeric input: sign alone ("-" for %u, "+" for %d) must not assign (ret <= 0, target untouched);
 *        "0x" for %x returns <= 1 and, if 0, leaves the target untouched; out-of-range "%d" input only asserted to return 0 or 1.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;

static int xs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vss(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(d_out_of_range)
{
    int a = 5; int r = xs("99999999999", "%d", &a);
    CHECK(r == 0 || r == 1);
}
TC_CASE(u_minus_alone)
{
    unsigned a = 5; int r = xs("-", "%u", &a);
    CHECK(r <= 0); CHECK_EQ_U(a, 5);
}
TC_CASE(d_plus_alone)
{
    int a = 5; int r = xs("+", "%d", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 5);
}
TC_CASE(x_0x_alone)
{
    unsigned a = 5; int r = xs("0x", "%x", &a);
    CHECK(r <= 1); CHECK(r >= -1);
    if (r <= 0) CHECK_EQ_U(a, 5);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(d_out_of_range); TC_RUN(u_minus_alone); TC_RUN(d_plus_alone); TC_RUN(x_0x_alone);
    return tc_end();
}
