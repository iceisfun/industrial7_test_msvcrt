/* TC
 * id:    pff_neg_invalid_length_modifier_float_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size prefixes; C11 7.21.6.1 p7 (h/hh/ll/j/z/t/L are only defined for some conversions)
 * note:  PROBES whether ucrtbase's sprintf_s ACCEPTS (ignores) a length modifier that is not defined for the f conversion
 *        (h hh l ll j z t L w I32 I64) and prints the plain double, versus rejecting it via the invalid-parameter handler.
 *        Expected guess: all accepted, "1.500000", return 8, IPH count 0. A different outcome on real Windows means the
 *        assertion here is wrong (not the CRT). The formats are passed through a volatile pointer.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char buf[64];

static int try_fmt(const char *fmt, double v)
{
    const char * volatile f = fmt; int r;
    buf[0] = 'x'; buf[1] = 0; tc_iph_count = 0;
    r = sprintf_s(buf, sizeof buf, f, v);
    return r;
}
#define ACCEPT(fmt) do { int r = try_fmt(fmt, 1.5); CHECK_EQ_I(r, 8); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(buf, "1.500000"); } while (0)

TC_CASE(h_hh)    { ACCEPT("%hf"); ACCEPT("%hhf"); }
TC_CASE(l_ll)    { ACCEPT("%lf"); ACCEPT("%llf"); }
TC_CASE(L)       { ACCEPT("%Lf"); }
TC_CASE(j_z_t)   { ACCEPT("%jf"); ACCEPT("%zf"); ACCEPT("%tf"); }
TC_CASE(I_forms) { ACCEPT("%I32f"); ACCEPT("%I64f"); }
TC_CASE(w)       { ACCEPT("%wf"); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(h_hh); TC_RUN(l_ll); TC_RUN(L); TC_RUN(j_z_t); TC_RUN(I_forms); TC_RUN(w);
    return tc_end();
}
