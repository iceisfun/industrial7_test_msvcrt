/* TC
 * id:    scn_neg_sscanf_c_width_short_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 p10 (%Nc reads exactly N characters)
 * note:  PROBE. %5c with only 3 characters available: expected return EOF (input failure, nothing assigned), characters already
 *        read are stored in the destination ("abc" in d[0..2]), the following %n is not executed. Real behaviour may be 0 instead of EOF.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vs;
TC_CASE(c5_input_abc_ret) { char d[8]; int n, i; for (i = 0; i < 8; i++) d[i] = 'Z'; vs = "abc"; n = sscanf((const char *)vs, "%5c", d); CHECK_EQ_I(n, EOF); }
TC_CASE(c5_dest_state)
{
    char d[8]; int i; for (i = 0; i < 8; i++) d[i] = 'Z'; vs = "abc"; (void)sscanf((const char *)vs, "%5c", d);
    CHECK_EQ_I(d[0], 'a'); CHECK_EQ_I(d[1], 'b'); CHECK_EQ_I(d[2], 'c'); CHECK_EQ_I(d[3], 'Z'); CHECK_EQ_I(d[4], 'Z');
}
TC_CASE(c5_n_after) { char d[8]; int k = -7, n; vs = "abc"; n = sscanf((const char *)vs, "%5c%n", d, &k); CHECK(n <= 0); CHECK_EQ_I(k, -7); }
int main(void) { tc_begin(); TC_RUN(c5_input_abc_ret); TC_RUN(c5_dest_state); TC_RUN(c5_n_after); return tc_end(); }
