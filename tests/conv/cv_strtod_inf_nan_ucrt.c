/* TC
 * id:    cv_strtod_inf_nan_ucrt
 * area:  conv
 * funcs: 
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
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

TC_CASE(inf) { long o; CHECK(is_pinf(P("inf", &o))); CHECK_EQ_I(o, 3); }
TC_CASE(infinity) { long o; CHECK(is_pinf(P("INFINITY", &o))); CHECK_EQ_I(o, 8); }
TC_CASE(infin_partial) { long o; CHECK(is_pinf(P("infin", &o))); CHECK_EQ_I(o, 3); }
TC_CASE(neg_inf) { long o; double d = P("-inf", &o); CHECK(d < -1.7976931348623157e308); CHECK_EQ_I(o, 4); }
TC_CASE(nan) { long o; CHECK_IS_NAN(P("nan", &o)); CHECK_EQ_I(o, 3); }
TC_CASE(nan_payload) { long o; CHECK_IS_NAN(P("NaN(abc)", &o)); CHECK_EQ_I(o, 8); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");
    TC_RUN(inf); TC_RUN(infinity); TC_RUN(infin_partial); TC_RUN(neg_inf); TC_RUN(nan);
    TC_RUN(nan_payload);
    return tc_end();
}
