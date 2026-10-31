/* TC
 * id:    pff_nan_ucrt_variants_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS UCRT breaking changes (printf inf/nan C99 style)
 * note:  probed: -nan for negative quiet NaN with nonzero payload (ind only for exact 0xFFF8000000000000); %F/%E/%G uppercase whole text incl. -NAN(IND)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

TC_CASE(neg_quiet) { volatile double d = dfb(0xFFF8000000000001ULL); sprintf(b, "%f", d); CHECK_STR(b, "-nan"); }
TC_CASE(upper) { volatile double q = dfb(0x7FF8000000000000ULL), i = dfb(0xFFF8000000000000ULL);
    sprintf(b, "%F|%E|%G|%F", q, q, q, i); CHECK_STR(b, "NAN|NAN|NAN|-NAN(IND)"); }
int main(void) { tc_begin(); TC_RUN(neg_quiet); TC_RUN(upper); return tc_end(); }
