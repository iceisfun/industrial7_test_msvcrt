/* TC
 * id:    ucr_strtod_legacy_tokens_probe
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS legacy printf output spellings 1.#INF / 1.#QNAN / 1.#IND
 * note:  Probes whether ucrtbase strtod parses the legacy round-trip tokens printed by the old msvcrt (guess: yes).
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(inf) { char *e; double d; v_s = "1.#INF"; d = strtod(v_s, &e); CHECK(d > 1e308 && d == d * 2); CHECK_EQ_P(e, v_s + 6); }
TC_CASE(qnan) { char *e; double d; v_s = "1.#QNAN"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 7); }
TC_CASE(neg_ind) { char *e; double d; v_s = "-1.#IND"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 7); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(qnan); TC_RUN(neg_ind); return tc_end(); }
