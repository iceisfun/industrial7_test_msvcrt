/* TC
 * id:    pff_ecvt_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _ecvt_s
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
TC_CASE(pi4) { char b[100]; int d = -99, s = -99, r; volatile double v = 3.14159; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, v, 4, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "3142"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(small) { char b[100]; int d = -99, s = -99, r; volatile double v = 0.00123; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "12"); CHECK_EQ_I(d, -2); CHECK_EQ_I(s, 0); }
TC_CASE(negative) { char b[100]; int d = -99, s = -99, r; volatile double v = -5.0; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, v, 3, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "500"); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 1); }
TC_CASE(large) { char b[100]; int d = -99, s = -99, r; volatile double v = 12345.0; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, v, 3, &d, &s); CHECK_EQ_I(r, 0); CHECK_STR(b, "123"); CHECK_EQ_I(d, 5); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ecvt_s); CHECK(tc_p__ecvt_s != NULL);
  TC_RUN(pi4); TC_RUN(small); TC_RUN(negative); TC_RUN(large); return tc_end(); }
