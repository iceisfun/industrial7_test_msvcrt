/* TC
 * id:    scn_sscanf_scanset_edge_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.2 p12 (']' first in set is literal; '-' first/last is literal)
 * note:  PROBES that the Microsoft CRTs follow ISO for scanset edge syntax: "%[]a]" (leading ']' literal),
 *        "%[^]]" (leading ']' after '^' literal), "%[a-]" (trailing '-' literal). Not documented by MS.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(leading_bracket) { char a[16]; int n; v = "a]a]b"; n = sscanf((const char *)v, "%[]a]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "a]a]"); }
TC_CASE(neg_leading_bracket) { char a[16]; int n; v = "xy]z"; n = sscanf((const char *)v, "%[^]]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "xy"); }
TC_CASE(trailing_dash) { char a[16]; int n; v = "a-b-c"; n = sscanf((const char *)v, "%[a-]", a); CHECK_EQ_I(n, 1); CHECK_STR(a, "a-"); }
int main(void) { tc_begin(); TC_RUN(leading_bracket); TC_RUN(neg_leading_bracket); TC_RUN(trailing_dash); return tc_end(); }
