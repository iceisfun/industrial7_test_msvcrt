/* TC
 * id:    scn_sscanf_l_null_locale
 * area:  scanf
 * funcs: _sscanf_l
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _sscanf_l ("locale: the locale to use"), Locale-dependent behaviour of _l functions
 * note:  PROBE: a NULL _locale_t is expected to mean the current thread locale ("C" at start-up),
 *        so the result must match plain sscanf.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_int) { int x = 0, y = 0, r, r2; r = _sscanf_l("  17 rest", "%d", NULL, &x); r2 = sscanf("  17 rest", "%d", &y); CHECK_EQ_I(r, 1); CHECK_EQ_I(r2, 1); CHECK_EQ_I(x, 17); CHECK_EQ_I(x, y); }
TC_CASE(null_float_str) { double d = 0; char s[8]; int r; s[0] = 0; r = _sscanf_l("2.5 abc", "%lf %s", NULL, &d, s); CHECK_EQ_I(r, 2); CHECK_BITS_D(d, 2.5); CHECK_STR(s, "abc"); }

int main(void) { tc_begin(); TC_RUN(null_int); TC_RUN(null_float_str); return tc_end(); }
