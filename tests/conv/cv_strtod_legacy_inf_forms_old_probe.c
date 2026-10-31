/* TC
 * id:    cv_strtod_legacy_inf_forms_old_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   pre-C99 CRT strtod grammar (decimal digits only)
 * note:  Probes: old CRTs do not parse "1.#INF"; they read "1." as 1.0 and stop at the '#'.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(inf_form) { char *e = NULL; double d; vs = "1.#INF"; d = strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_I(e - vs, 2); }
TC_CASE(qnan_form) { char *e = NULL; double d; vs = "1.#QNAN"; d = strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_I(e - vs, 2); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(inf_form); TC_RUN(qnan_form); return tc_end(); }
