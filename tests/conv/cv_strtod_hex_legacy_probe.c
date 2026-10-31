/* TC
 * id:    cv_strtod_hex_legacy_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   pre-C99 CRT strtod grammar (decimal only)
 * note:  Probes: legacy CRTs do not know hex floats; "0x10" parses as 0 with endptr at the 'x', "0x1p4" likewise.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(hex_stops_at_x) { char *e = NULL; double d; vs = "0x10"; d = strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(e - vs, 1); }
TC_CASE(hex_p_exponent) { char *e = NULL; double d; vs = "0x1p4"; d = strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(e - vs, 1); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(hex_stops_at_x); TC_RUN(hex_p_exponent); return tc_end(); }
