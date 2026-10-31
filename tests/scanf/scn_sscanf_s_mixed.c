/* TC
 * id:    scn_sscanf_s_mixed
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: only %c %s %[ (and C S) take a size argument; numeric conversions and %n do not; %* suppressed fields take no pointer and no size
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(d_s_f) { int i = 0; char s[8]; float f = 0; int n; v = "42 hello 2.5"; n = sscanf_s((const char *)v, "%d %s %f", &i, s, (unsigned)8, &f); CHECK_EQ_I(n, 3); CHECK_EQ_I(i, 42); CHECK_STR(s, "hello"); CHECK_EQ_I(f == 2.5f, 1); }
TC_CASE(suppressed) { char s[8]; int n; v = "skip me"; n = sscanf_s((const char *)v, "%*s %s", s, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_STR(s, "me"); }
TC_CASE(n_directive) { char s[8]; int n, pos = -1; v = "abc def"; n = sscanf_s((const char *)v, "%s%n", s, (unsigned)8, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(pos, 3); }
TC_CASE(unsigned_hex_then_s) { unsigned u = 0; char s[4]; int n; v = "ff xyz"; n = sscanf_s((const char *)v, "%x %s", &u, s, (unsigned)4); CHECK_EQ_I(n, 2); CHECK_EQ_U(u, 255); CHECK_STR(s, "xyz"); }
int main(void) { tc_begin(); TC_RUN(d_s_f); TC_RUN(suppressed); TC_RUN(n_directive); TC_RUN(unsigned_hex_then_s); return tc_end(); }
