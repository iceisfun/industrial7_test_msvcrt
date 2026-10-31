/* TC
 * id:    scn_sscanf_width_str
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
static void memset_(char *p) { int i; for (i = 0; i < 8; i++) p[i] = 'Z'; }
TC_CASE(split) { char a[16], b[16]; int n; v = "abcdef"; n = sscanf((const char *)v, "%3s%s", a, b); CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_STR(b, "def"); }
TC_CASE(wide_width) { char a[16]; int n; v = "abc"; n = sscanf((const char *)v, "%10s", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); }
TC_CASE(sentinel) { char a[8]; int n; memset_(a); v = "abcdef"; n = sscanf((const char *)v, "%3s", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[3], 0); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z'); }
int main(void) { tc_begin(); TC_RUN(split); TC_RUN(wide_width); TC_RUN(sentinel); return tc_end(); }
