/* TC
 * id:    scn_neg_sscanf_c_eof_and_short
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 p3,p8,p12 (%c does not skip white space; input failure before any conversion returns EOF; else the count assigned)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vs;
TC_CASE(empty_c_eof) { char c = 'Z'; int n; vs = ""; n = sscanf((const char *)vs, "%c", &c); CHECK_EQ_I(n, EOF); CHECK_EQ_I(c, 'Z'); }
TC_CASE(d_then_c_eof_ret1) { int d = 0; char c = 'Z'; int n; vs = "5"; n = sscanf((const char *)vs, "%d%c", &d, &c); CHECK_EQ_I(n, 1); CHECK_EQ_I(d, 5); CHECK_EQ_I(c, 'Z'); }
TC_CASE(two_c_one_char) { char a = 'Y', b = 'Z'; int n; vs = "a"; n = sscanf((const char *)vs, "%c%c", &a, &b); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 'a'); CHECK_EQ_I(b, 'Z'); }
TC_CASE(c_reads_space) { char c = 'Z'; int n; vs = " "; n = sscanf((const char *)vs, "%c", &c); CHECK_EQ_I(n, 1); CHECK_EQ_I(c, ' '); }
TC_CASE(c_after_d_no_ws_skip_eof)
{
    int d = 0, n; char c = 'Z'; vs = "5  ";
    n = sscanf((const char *)vs, "%d %c", &d, &c);   /* the blank directive eats all spaces, then %c hits EOF */
    CHECK_EQ_I(n, 1); CHECK_EQ_I(d, 5); CHECK_EQ_I(c, 'Z');
    c = 'Z'; vs = "5 ";
    n = sscanf((const char *)vs, "%d%c", &d, &c);    /* no blank in format: %c takes the space */
    CHECK_EQ_I(n, 2); CHECK_EQ_I(c, ' ');
}
int main(void) { tc_begin(); TC_RUN(empty_c_eof); TC_RUN(d_then_c_eof_ret1); TC_RUN(two_c_one_char); TC_RUN(c_reads_space); TC_RUN(c_after_d_no_ws_skip_eof); return tc_end(); }
