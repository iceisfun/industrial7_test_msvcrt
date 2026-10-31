/* TC
 * id:    pfm_1_neg_probe_msvcr_capf_a_conv_legacy
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr100 msvcr90 msvcr80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field characters (%F, %a, %A documented for VS2013+/UCRT)
 * note:  PROBE: before VS2013 %F, %a and %A are not valid conversions; the VS2005+ CRT validates the format and is guessed to
 *        call the invalid-parameter handler and return -1 (a 'F' is a legacy far-modifier, so "%F" ends in a non-final state;
 *        %a/%A are unknown type characters). Each case records the result and iph count; msvcrt.dll not covered.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile fF  = "%F";
static const char * volatile fF10 = "%10F";
static const char * volatile fa  = "%a";
static const char * volatile fA  = "%A";

static void one(const char *volatile f)
{
    char b[64]; int n; b[0] = 0;
    tc_iph_count = 0;
    n = sprintf(b, f, 1.5);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cap_F)       { one(fF); }
TC_CASE(cap_F_width) { one(fF10); }
TC_CASE(lower_a)     { one(fa); }
TC_CASE(upper_A)     { one(fA); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(cap_F); TC_RUN(cap_F_width); TC_RUN(lower_a); TC_RUN(upper_A);
    return tc_end();
}
