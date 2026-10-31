/* TC
 * id:    scn_sscanf_scanset_range
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
TC_CASE(az) { char a[16]; int n; v = "hello123"; n = sscanf((const char *)v, "%[a-z]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "hello"); }
TC_CASE(digits) { char a[16]; int n; v = "2024abc"; n = sscanf((const char *)v, "%[0-9]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "2024"); }
TC_CASE(ident) { char a[16]; int n; v = "Ab_9-x"; n = sscanf((const char *)v, "%[a-zA-Z0-9_]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "Ab_9"); }
TC_CASE(neg_digits) { char a[16]; int n; v = "abc123"; n = sscanf((const char *)v, "%[^0-9]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); }
int main(void) { tc_begin(); TC_RUN(az); TC_RUN(digits); TC_RUN(ident); TC_RUN(neg_digits); return tc_end(); }
