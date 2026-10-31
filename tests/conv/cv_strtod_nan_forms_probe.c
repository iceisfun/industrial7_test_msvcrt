/* TC
 * id:    cv_strtod_nan_forms_probe
 * area:  conv
 * funcs: 
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * note:  Probes: strtod("nan(") stops after "nan" (standard grammar; UCRT handling of an unterminated n-char-sequence unverified);
 *        "-nan" returns NaN with sign bit set (C does not specify NaN sign).
 * ref:   C11 7.22.1.3 (INF, INFINITY, NAN, NAN(n-char-sequence), case-insensitive); MS docs strtod
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
static double P(const char *s, long *off) { char *e = NULL; double d; vs = s; d = strtod(vs, &e); *off = (long)(e - s); return d; }
static int is_pinf(double d) { return d > 1.7976931348623157e308; }

TC_CASE(nan_unterminated) { long o; CHECK_IS_NAN(P("nan(", &o)); CHECK_EQ_I(o, 3); }
TC_CASE(neg_nan_sign) { long o; double d = P("-nan", &o); CHECK_IS_NAN(d); CHECK(tc_dbits(d) >> 63); CHECK_EQ_I(o, 4); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");
    TC_RUN(nan_unterminated); TC_RUN(neg_nan_sign); 
    return tc_end();
}
