/* TC
 * id:    scn_sscanf_inf_nan_msvcr120_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf (VS2013)
 * note:  PROBE: VS2013 added C99 library features and msvcr120 scanf may already parse inf/nan; unsure. Accepts either outcome: (r==1 and value is inf/nan) or (r==0 and destination untouched). Real Windows result is the data.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(inf) { double d = 5.0; int r; v = "inf"; r = sscanf((const char *)v, "%lf", &d);
    if (r == 1) CHECK_EQ_U(tc_dbits(d), 0x7FF0000000000000ULL); else { CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); } }
TC_CASE(nan) { double d = 5.0; int r; v = "nan"; r = sscanf((const char *)v, "%lf", &d);
    if (r == 1) CHECK_IS_NAN(d); else { CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 5.0); } }
int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(nan); return tc_end(); }
