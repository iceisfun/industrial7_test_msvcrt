/* TC
 * id:    pfi_int_prec_alt_octal_zero_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (# with o increases precision so first digit is 0)
 * note:  Probes whether %#.0o of 0 yields "0" as ISO C requires. MS docs only say "0 is prepended to
 *        nonzero output value", so Windows may yield "". Likewise %#.0x of 0 (standard: "").
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(alt_o_prec0) { char b[16]; volatile unsigned z = 0; int n = sprintf(b, "%#.0o", z); CHECK_STR(b, "0"); CHECK_EQ_I(n, 1); }
TC_CASE(alt_x_prec0) { char b[16]; volatile unsigned z = 0; int n = sprintf(b, "%#.0x", z); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }

int main(void) { tc_begin(); TC_RUN(alt_o_prec0); TC_RUN(alt_x_prec0); return tc_end(); }
