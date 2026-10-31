/* TC
 * id:    scn_sscanf_char_c
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
static void fill(char *p) { int i; for (i = 0; i < 8; i++) p[i] = 'Z'; }
TC_CASE(no_skip) { char c = 0; int n; v = " x"; n = sscanf((const char *)v, "%c", &c); CHECK_EQ_I(n, 1); CHECK_EQ_I(c, ' '); }
TC_CASE(three) { char a[8]; int n; fill(a); v = "abcdef"; n = sscanf((const char *)v, "%3c", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[0], 'a'); CHECK_EQ_I(a[1], 'b'); CHECK_EQ_I(a[2], 'c'); CHECK_EQ_I(a[3], 'Z'); }
TC_CASE(c_c_c) { char a = 0, b = 0, c = 0; int n; v = "xyz"; n = sscanf((const char *)v, "%c%c%c", &a, &b, &c); CHECK_EQ_I(n, 3); CHECK_EQ_I(a, 'x'); CHECK_EQ_I(b, 'y'); CHECK_EQ_I(c, 'z'); }
TC_CASE(space_skips) { char c = 0; int n; v = " \t\n q"; n = sscanf((const char *)v, " %c", &c); CHECK_EQ_I(n, 1); CHECK_EQ_I(c, 'q'); }
TC_CASE(one) { char a[4]; int n; fill(a); v = "hi"; n = sscanf((const char *)v, "%1c", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[0], 'h'); CHECK_EQ_I(a[1], 'Z'); }
int main(void) { tc_begin(); TC_RUN(no_skip); TC_RUN(three); TC_RUN(c_c_c); TC_RUN(space_skips); TC_RUN(one); return tc_end(); }
