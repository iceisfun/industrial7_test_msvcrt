/* TC
 * id:    pfp_pct_literal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 p8 (%% conversion)
 * note:  
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(single)  { char b[16]; int n = sprintf(b, "%%"); CHECK_STR(b, "%"); CHECK_EQ_I(n, 1); }
TC_CASE(text)    { char b[32]; int n = sprintf(b, "100%% done"); CHECK_STR(b, "100% done"); CHECK_EQ_I(n, 9); }
TC_CASE(double_) { char b[16]; int n = sprintf(b, "%%%%"); CHECK_STR(b, "%%"); CHECK_EQ_I(n, 2); }
TC_CASE(mix)     { char b[32]; volatile int v = 50; int n = sprintf(b, "%d%%|%%%d", v, v); CHECK_STR(b, "50%|%50"); CHECK_EQ_I(n, 7); }

int main(void) { tc_begin(); TC_RUN(single); TC_RUN(text); TC_RUN(double_); TC_RUN(mix); return tc_end(); }
