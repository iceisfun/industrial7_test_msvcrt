/* TC
 * id:    cv_strtod_d_exponent_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.3 grammar allows only e/E (and p/P for hex) as exponent markers
 * note:  Probes: UCRT strtod("1d5") stops at 'd' (value 1, endptr+1). Pre-UCRT CRTs historically accepted 'd'/'D'
 *        as a Fortran-style exponent letter; not asserted for them.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(d_not_exponent) { char *e = NULL; double d; vs = "1d5"; d = strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_I(e - vs, 1); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(d_not_exponent); return tc_end(); }
