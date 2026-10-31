/* TC
 * id:    scn_neg_sscanf_n_only_empty_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2p16 (EOF only if input failure occurs before the first conversion; %n does not read input)
 * note:  Probes sscanf("", "%n", &n): believed to return 0 (not EOF) with n stored as 0.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *vin;

TC_CASE(n_only_empty_input_ret) { int n = 77, r; vin = ""; r = sscanf((const char *)vin, "%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 0); }

int main(void) { tc_begin(); TC_RUN(n_only_empty_input_ret); return tc_end(); }
