/* TC
 * id:    scn_sscanf_int_i_base
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
TC_CASE(decimal) { int a = 0, n; v = "42"; n = sscanf((const char *)v, "%i", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 42); }
TC_CASE(octal) { int a = 0, n; v = "017"; n = sscanf((const char *)v, "%i", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 15); }
TC_CASE(hex) { int a = 0, b = 0, n; v = "0x1F 0X1f"; n = sscanf((const char *)v, "%i %i", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 31); CHECK_EQ_I(b, 31); }
TC_CASE(negative_hex) { int a = 0, n; v = "-0x10"; n = sscanf((const char *)v, "%i", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, -16); }
int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(octal); TC_RUN(hex); TC_RUN(negative_hex); return tc_end(); }
