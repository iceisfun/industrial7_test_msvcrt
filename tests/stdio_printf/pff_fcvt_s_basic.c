/* TC
 * id:    pff_fcvt_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ecvt_s, _fcvt_s
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_CASE(pi2) { char b[100]; int d = -99, s = -99, r; volatile double v = 3.14159; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 100, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(negative) { char b[100]; int d = -99, s = -99, r; volatile double v = -3.14159; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 100, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "314"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 1); }
TC_CASE(ndigit0) { char b[100]; int d = -99, s = -99, r; volatile double v = 12.7; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 100, v, 0, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "13"); CHECK_EQ_I(d, 2); CHECK_EQ_I(s, 0); }
TC_CASE(more_digits) { char b[100]; int d = -99, s = -99, r; volatile double v = 1234.5; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 100, v, 3, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "1234500"); CHECK_EQ_I(d, 4); }
int main(void) { tc_begin(); TC_LOAD_PROC(_fcvt_s); CHECK(tc_p__fcvt_s != NULL);
  TC_RUN(pi2); TC_RUN(negative); TC_RUN(ndigit0); TC_RUN(more_digits); return tc_end(); }
