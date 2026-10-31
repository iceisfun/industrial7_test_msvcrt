/* TC
 * id:    scn_sscanf_inf_nan_legacy_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf (pre-VS2015)
 * note:  PROBE: pre-UCRT scanf did not parse C99 inf/nan: matching failure, return 0, destination untouched. '-1.#INF' result recorded loosely (0 or 1).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(inf_fails) { double d = 5.0; int r; v = "inf"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(nan_fails) { double d = 5.0; int r; v = "nan"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); }
TC_CASE(hash_inf_record) { double d = 5.0; int r; v = "-1.#INF"; r = sscanf((const char *)v, "%lf", &d); CHECK(r == 0 || r == 1); }
int main(void) { tc_begin(); TC_RUN(inf_fails); TC_RUN(nan_fails); TC_RUN(hash_inf_record); return tc_end(); }
