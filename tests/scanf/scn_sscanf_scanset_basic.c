/* TC
 * id:    scn_sscanf_scanset_basic
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
TC_CASE(abc) { char a[16]; int n, pos = -1; v = "abcabd"; n = sscanf((const char *)v, "%[abc]%n", a, &pos); CHECK_EQ_I(n, 1); CHECK_STR(a, "abcab"); CHECK_EQ_I(pos, 5); }
TC_CASE(negated) { char a[16], b[16]; int n; v = "hello world,rest"; n = sscanf((const char *)v, "%[^,],%s", a, b); CHECK_EQ_I(n, 2); CHECK_STR(a, "hello world"); CHECK_STR(b, "rest"); }
TC_CASE(no_ws_skip) { char a[16] = "init"; int n; v = " abc"; n = sscanf((const char *)v, "%[abc]", a); CHECK_EQ_I(n, 0); CHECK_STR(a, "init"); }
TC_CASE(nul) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Q'; v = "aab"; n = sscanf((const char *)v, "%[a]", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[2], 0); CHECK_EQ_I(a[3], 'Q'); }
TC_CASE(case_sensitive) { char a[16]; int n; v = "abAB"; n = sscanf((const char *)v, "%[ab]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); }
int main(void) { tc_begin(); TC_RUN(abc); TC_RUN(negated); TC_RUN(no_ws_skip); TC_RUN(nul); TC_RUN(case_sensitive); return tc_end(); }
