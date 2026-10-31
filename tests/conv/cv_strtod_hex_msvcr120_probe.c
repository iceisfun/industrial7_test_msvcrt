/* TC
 * id:    cv_strtod_hex_msvcr120_probe
 * area:  conv
 * funcs: 
 * procs: strtod
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   VS2013 C99 library conformance announcement
 * note:  Probes: msvcr120 (VS2013) strtod parses hexadecimal floating point ("0x1p4" == 16, "0x10" == 16).
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(hex_with_exponent) { char *e = NULL; double d; vs = "0x1p4"; d = strtod(vs, &e); CHECK_BITS_D(d, 16.0); CHECK_EQ_I(e - vs, 5); }
TC_CASE(hex_no_exponent) { char *e = NULL; double d; vs = "0x10"; d = strtod(vs, &e); CHECK_BITS_D(d, 16.0); CHECK_EQ_I(e - vs, 4); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); TC_RUN(hex_with_exponent); TC_RUN(hex_no_exponent); return tc_end(); }
