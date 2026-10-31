/* TC
 * id:    cv_strtod_legacy_inf_forms_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS printf output forms "1.#INF", "1.#QNAN", "-1.#IND" (docs: printf, "Infinity and NaN output")
 * note:  Probes: UCRT strtod round-trips the Microsoft printf spellings (1.#INF -> +inf, 1.#QNAN -> nan,
 *        -1.#IND -> negative nan). Counterpart for old CRTs: cv_strtod_legacy_inf_forms_old_probe.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(inf) { char *e = NULL; double d; vs = "1.#INF"; d = strtod(vs, &e); CHECK(d > 1.7976931348623157e308); CHECK_EQ_I(e - vs, 6); }
TC_CASE(qnan) { char *e = NULL; double d; vs = "1.#QNAN"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_I(e - vs, 7); }
TC_CASE(neg_ind) { char *e = NULL; double d; vs = "-1.#IND"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK(tc_dbits(d) >> 63); CHECK_EQ_I(e - vs, 7); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(inf); TC_RUN(qnan); TC_RUN(neg_ind); return tc_end(); }
