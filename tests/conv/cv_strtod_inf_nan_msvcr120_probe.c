/* TC
 * id:    cv_strtod_inf_nan_msvcr120_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   VS2013 C99 library conformance announcement
 * note:  Probes: msvcr120 strtod accepts C99 "inf", "nan" and "infinity" (with full endptr).
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(inf) { char *e = NULL; double d; vs = "inf"; d = strtod(vs, &e); CHECK(d > 1.7976931348623157e308); CHECK_EQ_I(e - vs, 3); }
TC_CASE(nan) { char *e = NULL; double d; vs = "nan"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_I(e - vs, 3); }
TC_CASE(infinity) { char *e = NULL; double d; vs = "infinity"; d = strtod(vs, &e); CHECK(d > 1.7976931348623157e308); CHECK_EQ_I(e - vs, 8); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(inf); TC_RUN(nan); TC_RUN(infinity); return tc_end(); }
