/* TC
 * id:    scn_sscanf_hex_x
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2; MS docs scanf type field characters
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

static volatile const char *v;
TC_CASE(case_insens) { unsigned a = 0, b = 0, c = 0; int n; v = "ff FF Ff"; n = sscanf((const char *)v, "%x %X %x", &a, &b, &c); CHECK_EQ_I(n, 3); CHECK_EQ_U(a, 255); CHECK_EQ_U(b, 255); CHECK_EQ_U(c, 255); }
TC_CASE(prefix) { unsigned a = 0, b = 0; int n; v = "0xff 0XAb"; n = sscanf((const char *)v, "%x %x", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 255); CHECK_EQ_U(b, 0xab); }
TC_CASE(mixed) { unsigned a = 0; int n; v = "1aF9"; n = sscanf((const char *)v, "%x", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0x1af9); }
TC_CASE(stops_at_g) { unsigned a = 0; int n, pos = -1; char c = 0; v = "12g"; n = sscanf((const char *)v, "%x%c%n", &a, &c, &pos); CHECK_EQ_I(n, 2); CHECK_EQ_U(a, 0x12); CHECK_EQ_I(c, 'g'); CHECK_EQ_I(pos, 3); }
TC_CASE(eight_digits) { unsigned a = 0; int n; v = "ffffffff"; n = sscanf((const char *)v, "%x", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 0xffffffffu); }
int main(void) { tc_begin(); TC_RUN(case_insens); TC_RUN(prefix); TC_RUN(mixed); TC_RUN(stops_at_g); TC_RUN(eight_digits); return tc_end(); }
