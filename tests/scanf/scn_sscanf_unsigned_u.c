/* TC
 * id:    scn_sscanf_unsigned_u
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
TC_CASE(big) { unsigned a = 0; int n; v = "4000000000"; n = sscanf((const char *)v, "%u", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 4000000000u); }
TC_CASE(max) { unsigned a = 0; int n; v = "4294967295"; n = sscanf((const char *)v, "%u", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, UINT_MAX); }
TC_CASE(minus_one) { unsigned a = 0; int n; v = "-1"; n = sscanf((const char *)v, "%u", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, UINT_MAX); }
TC_CASE(plus) { unsigned a = 0; int n; v = "+5"; n = sscanf((const char *)v, "%u", &a); CHECK_EQ_I(n, 1); CHECK_EQ_U(a, 5); }
int main(void) { tc_begin(); TC_RUN(big); TC_RUN(max); TC_RUN(minus_one); TC_RUN(plus); return tc_end(); }
