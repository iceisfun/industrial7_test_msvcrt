/* TC
 * id:    pfm_3_conv_capc_prec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf precision specification (precision has no effect on c/C)
 * note:  PROBE: precision (.0, bare '.', .3) is ignored for %C, so the character is always printed. The alternative would be an empty
 *        output for .0 (as for %.0s).
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_p0 = "[%.0C]";
static const char * volatile f_pdot = "[%.C]";
static const char * volatile f_p3 = "[%.3C]";
static const char * volatile f_p0w = "[%3.0C]";

TC_CASE(prec0)    { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_p0, c);   CHECK_STR(b, "[A]"); CHECK_EQ_I(r, 3); }
TC_CASE(bare_dot) { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_pdot, c); CHECK_STR(b, "[A]"); CHECK_EQ_I(r, 3); }
TC_CASE(prec3)    { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_p3, c);   CHECK_STR(b, "[A]"); CHECK_EQ_I(r, 3); }
TC_CASE(prec0_width) { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_p0w, c); CHECK_STR(b, "[  A]"); CHECK_EQ_I(r, 5); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(prec0); TC_RUN(bare_dot); TC_RUN(prec3); TC_RUN(prec0_width);
    return tc_end();
}
