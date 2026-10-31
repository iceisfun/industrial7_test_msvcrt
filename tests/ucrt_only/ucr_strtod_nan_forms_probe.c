/* TC
 * id:    ucr_strtod_nan_forms_probe
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.1.3 (NAN(n-char-seq) is accepted by the standard; UCRT's handling is unverified)
 * note:  Probes (1) whether "-nan" yields a NaN with the sign bit set, (2) whether "nan(123)" consumes the whole
 *        parenthesised sequence (guess: UCRT only recognises nan(snan)/nan(ind) and may stop after "nan").
 *        Each guess is its own case.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(neg_nan_sign_bit) { char *e; double d; v_s = "-nan"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK((tc_dbits(d) >> 63) == 1); }
TC_CASE(nan_payload_consumed) { char *e; double d; v_s = "nan(123)"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 8); }
TC_CASE(nan_ind_consumed) { char *e; double d; v_s = "nan(ind)"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 8); }

int main(void) { tc_begin(); TC_RUN(neg_nan_sign_bit); TC_RUN(nan_payload_consumed); TC_RUN(nan_ind_consumed); return tc_end(); }
