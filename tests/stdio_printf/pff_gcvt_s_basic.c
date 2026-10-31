/* TC
 * id:    pff_gcvt_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _gcvt_s: fixed notation unless exponent < -4 or >= digits
 * note:  exponent digit count differs (3 on legacy CRTs, 2 on ucrt); the test parses the exponent instead of comparing text
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
TC_CASE(simple) { char b[32]; int r; volatile double v = 1.5; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 6); CHECK_EQ_I(r, 0); CHECK_STR(b, "1.5"); }
TC_CASE(small_fixed) { char b[32]; int r; volatile double v = 0.0001234; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 5); CHECK_EQ_I(r, 0); CHECK_STR(b, "0.0001234"); }
TC_CASE(round6) { char b[32]; int r; volatile double v = 1234.5678; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 6); CHECK_EQ_I(r, 0); CHECK_STR(b, "1234.57"); }
TC_CASE(exponent_form) { char b[32]; int r, e = 0; const char *p; volatile double v = 1.5e10; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 3); CHECK_EQ_I(r, 0); CHECK(strncmp(b, "1.5e+", 5) == 0);
  for (p = b + 5; *p >= '0' && *p <= '9'; p++) e = e * 10 + (*p - '0');
  CHECK_EQ_I(e, 10); CHECK(*p == 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gcvt_s); CHECK(tc_p__gcvt_s != NULL);
  TC_RUN(simple); TC_RUN(small_fixed); TC_RUN(round6); TC_RUN(exponent_form); return tc_end(); }
