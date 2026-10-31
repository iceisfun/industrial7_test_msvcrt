/* TC
 * id:    scn_neg_sscanf_scanset_nomatch
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p12 ([ matches a non-empty sequence), p16
 * note:  [ does not skip leading whitespace; a zero-length match is a matching failure and assigns nothing. Empty input is an input failure (EOF).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

TC_CASE(digits_vs_letters) { char s[4] = {'a','b','c',0}; int r; in = "123"; r = sscanf(in, "%[abc]", s); CHECK_EQ_I(r, 0); CHECK_STR(s, "abc"); }
TC_CASE(negated_all_excluded) { char s[4] = {'a','b','c',0}; int r; in = "abc"; r = sscanf(in, "%[^abc]", s); CHECK_EQ_I(r, 0); CHECK_STR(s, "abc"); }
TC_CASE(empty_input) { char s[4] = {'a','b','c',0}; int r; in = ""; r = sscanf(in, "%[a]", s); CHECK_EQ_I(r, EOF); CHECK_STR(s, "abc"); }
TC_CASE(no_ws_skip) { char s[4] = {'a','b','c',0}; int r, n = -1; in = "  x"; r = sscanf(in, "%[x]%n", s, &n); CHECK_EQ_I(r, 0); CHECK_STR(s, "abc"); CHECK_EQ_I(n, -1); }
TC_CASE(ws_in_set_ok) { char s[8]; int r; in = "  x"; r = sscanf(in, "%[ x]", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "  x"); }

int main(void) { tc_begin(); TC_RUN(digits_vs_letters); TC_RUN(negated_all_excluded); TC_RUN(empty_input); TC_RUN(no_ws_skip); TC_RUN(ws_in_set_ok); return tc_end(); }
