/* TC
 * id:    pff_cvt_zero_value_probe
 * area:  stdio_printf
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ecvt_s/_fcvt_s (zero not specifically documented)
 * note:  probed: zero yields an all-'0' non-empty digit string, sign 0, rc 0; decpt deliberately not asserted
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
static int allzero(const char *s) { size_t i, n = strlen(s); if (!n) return 0; for (i = 0; i < n; i++) if (s[i] != '0') return 0; return 1; }
TC_CASE(ecvt) { char b[100]; int d = -99, s = -99, r; volatile double v = 0.0; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, v, 3, &d, &s); CHECK_EQ_I(r, 0); CHECK(allzero(b)); CHECK_EQ_I(s, 0); }
TC_CASE(fcvt) { char b[100]; int d = -99, s = -99, r; volatile double v = 0.0; if (!tc_p__fcvt_s) TC_SKIP("none");
  r = tc_p__fcvt_s(b, 100, v, 2, &d, &s); CHECK_EQ_I(r, 0); CHECK(allzero(b)); CHECK_EQ_I(s, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s); CHECK(tc_p__ecvt_s && tc_p__fcvt_s);
  TC_RUN(ecvt); TC_RUN(fcvt); return tc_end(); }
