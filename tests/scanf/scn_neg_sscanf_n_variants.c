/* TC
 * id:    scn_neg_sscanf_n_variants
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (%n stores chars consumed so far, does not count as a conversion); p3 (failed directive stops processing)
 * note:  %n after a failed directive is never reached, so the target stays untouched and the return value reflects only earlier successful assignments. The "%n only on empty input" return value is a probe in scn_neg_sscanf_n_only_empty_probe.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *vin;

TC_CASE(n_after_set_nomatch_untouched) { char b[8] = "Q"; int n = 77, r; vin = "123"; r = sscanf((const char *)vin, "%[a-z]%n", b, &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 77); }
TC_CASE(n_after_c_eof_untouched) { char c = 'Q'; int n = 77, r; vin = ""; r = sscanf((const char *)vin, "%c%n", &c, &n); CHECK_EQ_I(r, EOF); CHECK_EQ_I(n, 77); CHECK_EQ_I(c, 'Q'); }
TC_CASE(n_after_literal_fail_untouched) { int n = 77, r; vin = "xyz"; r = sscanf((const char *)vin, "abc%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 77); }
TC_CASE(n_after_suppress_fail) { int n = 77, r; vin = "abc"; r = sscanf((const char *)vin, "%*d%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 77); }
TC_CASE(d_mismatch_then_n) { int a = 5, n = 77, r; vin = "xyz"; r = sscanf((const char *)vin, "%d%n", &a, &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 5); CHECK_EQ_I(n, 77); }
TC_CASE(n_not_counted_after_fail) { int a = 1, b = 2, n = 77, r; vin = "5 x"; r = sscanf((const char *)vin, "%d%n%d%n", &a, &n, &b, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(n, 1); CHECK_EQ_I(b, 2); }

int main(void) { tc_begin(); TC_RUN(n_after_set_nomatch_untouched); TC_RUN(n_after_c_eof_untouched); TC_RUN(n_after_literal_fail_untouched);
    TC_RUN(n_after_suppress_fail); TC_RUN(d_mismatch_then_n); TC_RUN(n_not_counted_after_fail); return tc_end(); }
