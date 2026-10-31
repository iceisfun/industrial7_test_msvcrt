/* TC
 * id:    scn_sscanf_len_h
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2p11 (h modifier); MS docs scanf size specifiers
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(hd_max) { short a[3] = {0x1111, 0, 0x2222}; int n = sscanf("32767", "%hd", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], 32767); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(hd_min) { short a[3] = {0x1111, 0, 0x2222}; int n = sscanf("-32768", "%hd", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[1], -32768); CHECK_EQ_I(a[0], 0x1111); CHECK_EQ_I(a[2], 0x2222); }
TC_CASE(hu_max) { unsigned short a[3] = {0x1111, 0, 0x2222}; int n = sscanf("65535", "%hu", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 65535); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(hx_ffff) { unsigned short a[3] = {0x1111, 0, 0x2222}; int n = sscanf("ffff", "%hx", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0xffff); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }
TC_CASE(ho) { unsigned short a[3] = {0x1111, 0, 0x2222}; int n = sscanf("777", "%ho", &a[1]); CHECK_EQ_I(n, 1); CHECK_EQ_U(a[1], 0777); CHECK_EQ_U(a[0], 0x1111); CHECK_EQ_U(a[2], 0x2222); }

int main(void) { tc_begin(); TC_RUN(hd_max); TC_RUN(hd_min); TC_RUN(hu_max); TC_RUN(hx_ffff); TC_RUN(ho); return tc_end(); }
