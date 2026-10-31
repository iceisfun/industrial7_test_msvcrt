/* TC
 * id:    scn_sscanf_s_string
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s, _scanf_s_l, wscanf_s, _wscanf_s_l (buffer size argument follows each %s; size in characters, includes the NUL)
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
static void fill(char *a, int n) { int i; for (i = 0; i < n; i++) a[i] = 'Z'; }
TC_CASE(basic) { char a[8]; int n; fill(a, 8); v = "abc"; n = sscanf_s((const char *)v, "%s", a, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); }
TC_CASE(exact_fit) { char a[8]; int n; fill(a, 8); v = "abc"; n = sscanf_s((const char *)v, "%s", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(a[4], 'Z'); }
TC_CASE(two_each_with_size) { char a[8], b[6]; int n; fill(a, 8); fill(b, 6); v = "hello world"; n = sscanf_s((const char *)v, "%s %s", a, (unsigned)8, b, (unsigned)6); CHECK_EQ_I(n, 2); CHECK_STR(a, "hello"); CHECK_STR(b, "world"); }
TC_CASE(width_and_size) { char a[8]; int n, pos = -1; fill(a, 8); v = "abcdef"; n = sscanf_s((const char *)v, "%3s%n", a, (unsigned)8, &pos); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(pos, 3); }
TC_CASE(nul_written) { char a[8]; int n; fill(a, 8); v = "xy z"; n = sscanf_s((const char *)v, "%s", a, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[0], 'x'); CHECK_EQ_I(a[1], 'y'); CHECK_EQ_I(a[2], 0); CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[7], 'Z'); }
int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(exact_fit); TC_RUN(two_each_with_size); TC_RUN(width_and_size); TC_RUN(nul_written); return tc_end(); }
