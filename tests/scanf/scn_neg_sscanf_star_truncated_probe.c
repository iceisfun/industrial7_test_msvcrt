/* TC
 * id:    scn_neg_sscanf_star_truncated_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 (invalid conversion specification is UB); MS docs scanf format specification
 * note:  Probes formats that end right after '*' / '*5' / '*l', and an unknown conversion type after '*'. Expectation: no crash, no assignment, ret <= 0; the unknown type '%*y' additionally calls the invalid-parameter handler once. For the truncated forms the iph count is only recorded as <= 1 (unknown).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static volatile const char *vin;

TC_CASE(star_end)       { int r, a = 99; tc_iph_install(); tc_iph_count = 0; vin = "123"; r = sscanf((const char *)vin, "%*", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK(tc_iph_count <= 1); }
TC_CASE(star_width_end) { int r, a = 99; tc_iph_install(); tc_iph_count = 0; vin = "123"; r = sscanf((const char *)vin, "%*5", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK(tc_iph_count <= 1); }
TC_CASE(star_l_end)     { int r, a = 99; tc_iph_install(); tc_iph_count = 0; vin = "123"; r = sscanf((const char *)vin, "%*l", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK(tc_iph_count <= 1); }
TC_CASE(star_unknown_type) { int r, a = 99; tc_iph_install(); tc_iph_count = 0; errno = 0; vin = "123"; r = sscanf((const char *)vin, "%*y", &a); CHECK(r <= 0); CHECK_EQ_I(a, 99); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }

int main(void) { tc_begin(); TC_RUN(star_end); TC_RUN(star_width_end); TC_RUN(star_l_end); TC_RUN(star_unknown_type); return tc_end(); }
