/* TC
 * id:    ucr_strtod_legacy_tokens_probe
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p2
 * conf:  observed
 * ref:   MS legacy printf output spellings 1.#INF / 1.#QNAN / 1.#IND
 * note:  Win10 22H2 (observed): ucrtbase strtod does not parse the legacy printf spellings 1.#INF / 1.#QNAN / -1.#IND; it converts the leading "1."
 *        (or "-1") and stops at '#' (end offsets 2, 2, 3). UCRT accepts only INF/INFINITY/NAN/NAN(...) (C99).
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

/* observed on Win10 22H2: the legacy msvcrt round-trip tokens are NOT recognised; only the leading "1." / "-1" is converted */
TC_CASE(inf) { char *e; double d; v_s = "1.#INF"; d = strtod(v_s, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_P(e, v_s + 2); }
TC_CASE(qnan) { char *e; double d; v_s = "1.#QNAN"; d = strtod(v_s, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_P(e, v_s + 2); }
TC_CASE(neg_ind) { char *e; double d; v_s = "-1.#IND"; d = strtod(v_s, &e); CHECK_BITS_D(d, -1.0); CHECK_EQ_P(e, v_s + 3); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(qnan); TC_RUN(neg_ind); return tc_end(); }
