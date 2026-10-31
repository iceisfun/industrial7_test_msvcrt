/* TC
 * id:    scn_sscanf_len_hh
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs scanf width specification (hh); C11 7.21.6.2p11
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(hhd_max) { signed char a[3] = {0x11, 0, 0x22}; int n = sscanf("127", "%hhd", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], 127); CHECK_EQ_I(a[0], 0x11); CHECK_EQ_I(a[2], 0x22); }
TC_CASE(hhd_min) { signed char a[3] = {0x11, 0, 0x22}; int n = sscanf("-128", "%hhd", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -128); CHECK_EQ_I(a[0], 0x11); CHECK_EQ_I(a[2], 0x22); }
TC_CASE(hhu) { unsigned char a[3] = {0x11, 0, 0x22}; int n = sscanf("255", "%hhu", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 255); CHECK_EQ_U(a[0], 0x11); CHECK_EQ_U(a[2], 0x22); }
TC_CASE(hhx) { unsigned char a[3] = {0x11, 0, 0x22}; int n = sscanf("ff", "%hhx", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xff); CHECK_EQ_U(a[0], 0x11); CHECK_EQ_U(a[2], 0x22); }

int main(void) { tc_begin(); TC_RUN(hhd_max); TC_RUN(hhd_min); TC_RUN(hhu); TC_RUN(hhx); return tc_end(); }
