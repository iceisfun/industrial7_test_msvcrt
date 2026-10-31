/* TC
 * id:    cv_strtoul_basic
 * area:  conv
 * funcs: strtoul

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoul
 * note:  strtoul("-1") negates modulo 2^32 -> ULONG_MAX without error (C11 7.22.1.4p5).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const char *volatile p;

TC_CASE(max) { char *e; errno = EDOM; p = "4294967295"; CHECK_EQ_U(strtoul(p, &e, 10), ULONG_MAX); CHECK_EQ_U(ULONG_MAX, 0xFFFFFFFFu); CHECK_EQ_P(e, p + 10); CHECK_EQ_I(errno, EDOM); }
TC_CASE(hex_max) { char *e; p = "0xFFFFFFFF"; CHECK_EQ_U(strtoul(p, &e, 16), 0xFFFFFFFFu); CHECK_EQ_P(e, p + 10); }
TC_CASE(base0) { char *e; p = "0x1F"; CHECK_EQ_U(strtoul(p, &e, 0), 31); p = "017"; CHECK_EQ_U(strtoul(p, &e, 0), 15); CHECK_EQ_P(e, p + 3); p = "17"; CHECK_EQ_U(strtoul(p, &e, 0), 17); }
TC_CASE(ws_plus) { char *e; p = " \t+25x"; CHECK_EQ_U(strtoul(p, &e, 10), 25); CHECK_EQ_P(e, p + 5); }
TC_CASE(neg_wraps) { char *e; errno = EDOM; p = "-1"; CHECK_EQ_U(strtoul(p, &e, 10), ULONG_MAX); CHECK_EQ_P(e, p + 2); CHECK_EQ_I(errno, EDOM); p = "-2"; CHECK_EQ_U(strtoul(p, &e, 10), ULONG_MAX - 1); }
TC_CASE(no_digits) { char *e = (char *)1; p = "q"; CHECK_EQ_U(strtoul(p, &e, 10), 0); CHECK_EQ_P(e, p); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(hex_max); TC_RUN(base0); TC_RUN(ws_plus); TC_RUN(neg_wraps); TC_RUN(no_digits); return tc_end(); }
