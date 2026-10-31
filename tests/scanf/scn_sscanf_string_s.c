/* TC
 * id:    scn_sscanf_string_s
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
TC_CASE(stops_at_ws) { char a[16]; int n, pos; const char *seps[6] = { "ab cd", "ab\tcd", "ab\ncd", "ab\rcd", "ab\vcd", "ab\fcd" }; int i;
    for (i = 0; i < 6; i++) { pos = -1; v = seps[i]; n = sscanf((const char *)v, "%15s%n", a, &pos); CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); CHECK_EQ_I(pos, 2); } }
TC_CASE(leading_ws) { char a[16]; int n; v = " \t\r\n\v\f word"; n = sscanf((const char *)v, "%s", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "word"); }
TC_CASE(punct) { char a[32]; int n; v = "a,b;c.d!"; n = sscanf((const char *)v, "%s", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "a,b;c.d!"); }
TC_CASE(nul_appended) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Q'; v = "xy z"; n = sscanf((const char *)v, "%s", a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a[2], 0); CHECK_EQ_I(a[3], 'Q'); }
TC_CASE(two) { char a[16], b[16]; int n; v = "  one   two  "; n = sscanf((const char *)v, "%s%s", a, b); CHECK_EQ_I(n, 2); CHECK_STR(a, "one"); CHECK_STR(b, "two"); }
int main(void) { tc_begin(); TC_RUN(stops_at_ws); TC_RUN(leading_ws); TC_RUN(punct); TC_RUN(nul_appended); TC_RUN(two); return tc_end(); }
