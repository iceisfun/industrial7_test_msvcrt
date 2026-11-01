/* TC
 * id:    cv_strtod_legacy_inf_forms_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS printf output forms "1.#INF", "1.#QNAN", "-1.#IND" (docs: printf, "Infinity and NaN output")
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): UCRT strtod does NOT parse the legacy Microsoft printf spellings: "1.#INF" and "1.#QNAN"
 *        parse as 1.0 with endptr after "1." (2 chars), "-1.#IND" as -1.0 with endptr after "-1." (3 chars). This is what ISO C requires
 *        (only inf/infinity/nan[(n-char-seq)] are special). Build-dependent: observed on 10.0.19045 ucrtbase only. Counterpart for old CRTs: cv_strtod_legacy_inf_forms_old_probe.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
TC_CASE(inf) { char *e = NULL; double d; vs = "1.#INF"; d = strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_I(e - vs, 2); }
TC_CASE(qnan) { char *e = NULL; double d; vs = "1.#QNAN"; d = strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_I(e - vs, 2); }
TC_CASE(neg_ind) { char *e = NULL; double d; vs = "-1.#IND"; d = strtod(vs, &e); CHECK_BITS_D(d, -1.0); CHECK_EQ_I(e - vs, 3); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(inf); TC_RUN(qnan); TC_RUN(neg_ind); return tc_end(); }
