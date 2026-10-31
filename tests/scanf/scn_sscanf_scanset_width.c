/* TC
 * id:    scn_sscanf_scanset_width
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
TC_CASE(width_then_s) { char a[16], b[16]; int n; v = "abcdef"; n = sscanf((const char *)v, "%3[a-z]%s", a, b); CHECK_EQ_I(n, 2); CHECK_STR(a, "abc"); CHECK_STR(b, "def"); }
TC_CASE(width_gt_match) { char a[16]; int n; v = "ab1"; n = sscanf((const char *)v, "%10[a-z]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); }
TC_CASE(sentinel) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; v = "abcdef"; n = sscanf((const char *)v, "%2[a-z]", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[2], 0); CHECK_EQ_I(a[3], 'Z'); }
int main(void) { tc_begin(); TC_RUN(width_then_s); TC_RUN(width_gt_match); TC_RUN(sentinel); return tc_end(); }
