/* TC
 * id:    cv_strtod_inf_nan_edge_ucrt
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

TC_CASE(in_no_conversion) { long o; CHECK_BITS_D(P("in", &o), 0.0); CHECK_EQ_I(o, 0); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");
    TC_RUN(in_no_conversion);
    return tc_end();
}
