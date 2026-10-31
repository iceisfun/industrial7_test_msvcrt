/* TC
 * id:    pff_cvt_special_probe
 * area:  stdio_printf
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ecvt_s (no inf/nan text documented)
 * note:  probed: the *cvt family keeps the legacy 1#INF / 1#QNAN / 1#IND spelling even on UCRT; only the prefix of the buffer, decpt==1 and rc==0 are asserted
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
static int starts(const char *s, const char *p) { return strncmp(s, p, strlen(p)) == 0; }
TC_CASE(ecvt_inf) { char b[100]; int d = -99, s = -99, r; double z = 0, v = 1.0 / z; if (!tc_p__ecvt_s) TC_SKIP("none");
  { volatile double w = v; v = w; } r = tc_p__ecvt_s(b, 100, v, 5, &d, &s); CHECK_EQ_I(r, 0); CHECK(starts(b, "1#INF")); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(fcvt_inf) { char b[100]; int d = -99, s = -99, r; double z = 0, v = 1.0 / z; if (!tc_p__fcvt_s) TC_SKIP("none");
  { volatile double w = v; v = w; } r = tc_p__fcvt_s(b, 100, v, 5, &d, &s); CHECK_EQ_I(r, 0); CHECK(starts(b, "1#INF")); CHECK_EQ_I(d, 1); }
TC_CASE(ecvt_qnan) { char b[100]; int d = -99, s = -99, r; union { unsigned long long u; double d; } x; x.u = 0x7FF8000000000001ULL; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, x.d, 8, &d, &s); CHECK_EQ_I(r, 0); CHECK(starts(b, "1#QNAN")); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 0); }
TC_CASE(ecvt_indefinite) { char b[100]; int d = -99, s = -99, r; union { unsigned long long u; double d; } x; x.u = 0xFFF8000000000000ULL; if (!tc_p__ecvt_s) TC_SKIP("none");
  r = tc_p__ecvt_s(b, 100, x.d, 8, &d, &s); CHECK_EQ_I(r, 0); CHECK(starts(b, "1#IND")); CHECK_EQ_I(d, 1); CHECK_EQ_I(s, 1); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s); CHECK(tc_p__ecvt_s && tc_p__fcvt_s);
  TC_RUN(ecvt_inf); TC_RUN(fcvt_inf); TC_RUN(ecvt_qnan); TC_RUN(ecvt_indefinite); return tc_end(); }
