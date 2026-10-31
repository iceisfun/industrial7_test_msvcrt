/* TC
 * id:    scn_sscanf_nan_ind_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf/scanf; UCRT text forms 1.#INF 1.#IND 1.#QNAN, nan(ind) nan(snan)
 * note:  PROBE: UCRT scanf accepts the round-trip forms printf produced on old CRTs / documents (1.#INF, 1.#IND, 1.#QNAN, 1.#SNAN, nan(ind), nan(snan)).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
#define INFB 0x7FF0000000000000ULL

TC_CASE(hash_inf) { double d = 0; int r; v = "1.#INF"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_dbits(d), INFB); }
TC_CASE(neg_hash_inf) { double d = 0; int r; v = "-1.#INF"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_dbits(d), INFB | 0x8000000000000000ULL); }
TC_CASE(neg_hash_ind) { double d = 0; int r; v = "-1.#IND"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_IS_NAN(d); }
TC_CASE(hash_qnan) { double d = 0; int r; v = "1.#QNAN"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_IS_NAN(d); }
TC_CASE(nan_ind) { double d = 0; int r; v = "nan(ind)"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_IS_NAN(d); }
TC_CASE(nan_ind_sign) { double d = 0; int r; v = "nan(ind)"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK((tc_dbits(d) >> 63) == 1); }
TC_CASE(nan_snan) { double d = 0; int r; v = "nan(snan)"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_IS_NAN(d); }
int main(void) { tc_begin(); TC_RUN(hash_inf); TC_RUN(neg_hash_inf); TC_RUN(neg_hash_ind); TC_RUN(hash_qnan); TC_RUN(nan_ind); TC_RUN(nan_ind_sign); TC_RUN(nan_snan); return tc_end(); }
