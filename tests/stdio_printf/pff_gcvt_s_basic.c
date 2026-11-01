/* TC
 * id:    pff_gcvt_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _gcvt_s (does not specify the fixed/scientific switch-over); the exponent < -4 rule is the C %g rule, not documented for _gcvt_s
 * note:  exponent digit count differs between CRTs; the test parses the exponent instead of comparing text.
 *        Win10 22H2 ucrtbase (observed, stable over repeated runs; _gcvt_s is not ISO C, the old exponent < -4 guess was a test bug; the switch-over is at exponent < -1, matching the legacy CRT gcvt rule): _gcvt_s(0.01234 / 0.0001234, 5 digits) -> "1.234e-002" / "1.234e-004" (3-digit exponent) instead of fixed notation; 0.1234 stays fixed. small_fixed uses 0.1234 (fixed), small_sci asserts the scientific form on ucrtbase.
 *        Raw evidence (baseline): TC FAIL pff_gcvt_s_basic::small_fixed ... got="1.234e-004" want="0.0001234"; probe (0.01234,5)->"1.234e-002", (0.1234,5)->"0.1234".
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
TC_CASE(small_fixed) { char b[32]; int r; volatile double v = 0.1234; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 5); CHECK_EQ_I(r, 0); CHECK_STR(b, "0.1234"); }
TC_CASE(small_sci) { char b[32]; int r; volatile double v = 0.01234, w = 0.0001234; if (!tc_p__gcvt_s) TC_SKIP("none");
#if TC_CRT_VER >= 140
  /* observed Win10 22H2 ucrtbase: anything below 0.1 already switches to scientific with a 3-digit exponent (C %g would keep 0.01234 / 0.0001234 fixed) */
  r = tc_p__gcvt_s(b, 32, v, 5); CHECK_EQ_I(r, 0); CHECK_STR(b, "1.234e-002");
  r = tc_p__gcvt_s(b, 32, w, 5); CHECK_EQ_I(r, 0); CHECK_STR(b, "1.234e-004");
#else
  TC_SKIP("scientific threshold only measured on ucrtbase");
#endif
}
TC_CASE(round6) { char b[32]; int r; volatile double v = 1234.5678; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 6); CHECK_EQ_I(r, 0); CHECK_STR(b, "1234.57"); }
TC_CASE(exponent_form) { char b[32]; int r, e = 0; const char *p; volatile double v = 1.5e10; if (!tc_p__gcvt_s) TC_SKIP("none");
  r = tc_p__gcvt_s(b, 32, v, 3); CHECK_EQ_I(r, 0); CHECK(strncmp(b, "1.5e+", 5) == 0);
  for (p = b + 5; *p >= '0' && *p <= '9'; p++) e = e * 10 + (*p - '0');
  CHECK_EQ_I(e, 10); CHECK(*p == 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_gcvt_s); CHECK(tc_p__gcvt_s != NULL);
  TC_RUN(simple); TC_RUN(small_fixed); TC_RUN(small_sci); TC_RUN(round6); TC_RUN(exponent_form); return tc_end(); }
