/* TC
 * id:    pfv_neg_invalid_flag_combo_modifier_mismatch
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Size specification (modifiers are defined for specific conversions only)
 * note:  PROBE: length modifiers applied to a conversion they are not defined for ("%hf", "%hhs", "%llc", "%lp"). Most likely
 *        outcome on the UCRT: the modifier is tolerated and ignored and the conversion behaves as without it (the other possibility
 *        is -1/EINVAL). One case per format so each outcome is recorded independently; nothing is mixed with documented checks.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_hf = "%hf", *volatile f_hhs = "%hhs", *volatile f_llc = "%llc", *volatile f_lp = "%lp";
static volatile double d = 1.5;
static const char *volatile s = "xyz";
static volatile int c = 'A';
static void *volatile p = (void *)(ULONG_PTR)0x1234;

TC_CASE(hf) { char b[64]; int r = sprintf(b, f_hf, d); CHECK_EQ_I(r, 8); CHECK_STR(b, "1.500000"); }
TC_CASE(hhs) { char b[64]; int r = sprintf(b, f_hhs, s); CHECK_EQ_I(r, 3); CHECK_STR(b, "xyz"); }
TC_CASE(llc) { char b[64]; int r = sprintf(b, f_llc, c); CHECK_EQ_I(r, 1); CHECK_STR(b, "A"); }
TC_CASE(lp)
{
    char b[64]; int r = sprintf(b, f_lp, p);
    CHECK_STR(b, TC_X64 ? "0000000000001234" : "00001234");
    CHECK_EQ_I(r, TC_X64 ? 16 : 8);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(hf); TC_RUN(hhs); TC_RUN(llc); TC_RUN(lp);
    return tc_end();
}
