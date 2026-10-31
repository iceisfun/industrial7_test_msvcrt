/* TC
 * id:    scn_neg_sscanf_huge_width_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs scanf width specification
 * note:  Probes absurd widths: must not crash or overread. Widths that overflow int ('%99999999999d', '%4294967296d', '%2147483648c') have unknown parse behaviour, so only "no crash, ret <= 1, and if a value was assigned it is correct" is asserted. '%2147483647d' and '%65536s' are plain large widths on short input and are believed to behave like an unbounded width.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *vin;

TC_CASE(d_width_gt_uint) { int a = 99, r; tc_iph_install(); vin = "123"; r = sscanf((const char *)vin, "%99999999999d", &a); CHECK(r <= 1); CHECK(r == 1 ? a == 123 : a == 99); }
TC_CASE(s_width_65536_short_input) { char b[16] = "Q"; int r; vin = "abc"; r = sscanf((const char *)vin, "%65536s", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); }
TC_CASE(d_width_int_max) { int a = 99, r; vin = "123"; r = sscanf((const char *)vin, "%2147483647d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 123); }
TC_CASE(c_width_huge_with_short_input) { char b[16]; int r, i; for (i = 0; i < 16; i++) b[i] = 'Z'; tc_iph_install(); vin = "ab"; r = sscanf((const char *)vin, "%2147483648c", b); CHECK(r <= 1); CHECK_EQ_I(b[8], 'Z'); CHECK_EQ_I(b[15], 'Z'); }

int main(void) { tc_begin(); TC_RUN(d_width_gt_uint); TC_RUN(s_width_65536_short_input); TC_RUN(d_width_int_max); TC_RUN(c_width_huge_with_short_input); return tc_end(); }
