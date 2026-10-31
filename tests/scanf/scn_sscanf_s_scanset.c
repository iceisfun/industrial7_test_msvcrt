/* TC
 * id:    scn_sscanf_s_scanset
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: size argument follows each %[ ] pointer, in characters including the NUL
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(range) { char a[16]; int n, pos = -1; v = "abc123"; n = sscanf_s((const char *)v, "%[a-z]%n", a, (unsigned)16, &pos); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(pos, 3); }
TC_CASE(negated) { char a[16], b[16]; int n; v = "foo,bar"; n = sscanf_s((const char *)v, "%[^,],%[^,]", a, (unsigned)16, b, (unsigned)16); CHECK_EQ_I(n, 2); CHECK_STR(a, "foo"); CHECK_STR(b, "bar"); }
TC_CASE(exact_fit) { char a[6]; int n, i; for (i = 0; i < 6; i++) a[i] = 'Z'; v = "abc!"; n = sscanf_s((const char *)v, "%[a-z]", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(a[4], 'Z'); }
TC_CASE(width_and_size) { char a[16]; int n; v = "abcdef"; n = sscanf_s((const char *)v, "%2[a-z]", a, (unsigned)16); CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); }
int main(void) { tc_begin(); TC_RUN(range); TC_RUN(negated); TC_RUN(exact_fit); TC_RUN(width_and_size); return tc_end(); }
