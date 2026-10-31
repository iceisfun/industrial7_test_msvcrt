/* TC
 * id:    pfv_g_large_precision_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   UCRT exact conversion; 0.1 = 0.1000000000000000055511151231257827...

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(f20_point1) { char b[128]; d = 0.1; sprintf(b, "%.20f", d); CHECK_STR(b, "0.10000000000000000555"); }
TC_CASE(f25_point1) { char b[128]; d = 0.1; sprintf(b, "%.25f", d); CHECK_STR(b, "0.1000000000000000055511151"); }
TC_CASE(g30_point1) { char b[128]; d = 0.1; sprintf(b, "%.30g", d); CHECK_STR(b, "0.100000000000000005551115123126"); }
TC_CASE(g17_point1) { char b[128]; d = 0.1; sprintf(b, "%.17g", d); CHECK_STR(b, "0.10000000000000001"); }
TC_CASE(e30_one_third) { char b[128]; d = 1.0 / 3.0; sprintf(b, "%.30e", d); CHECK_STR(b, "3.333333333333333148296162562474e-01"); }

int main(void) { tc_begin(); TC_RUN(f20_point1); TC_RUN(f25_point1); TC_RUN(g30_point1); TC_RUN(g17_point1); TC_RUN(e30_one_third); return tc_end(); }
