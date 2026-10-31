/* TC
 * id:    pfv_probe_inf_nan_pre140_misc
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   well-known pre-2015 MS CRT output
 * note:  PROBES: SNaN %f -> 1.#SNAN0; %g qNaN -> 1.#QNAN; %g indefinite -> -1.#IND; %+f inf -> +1.#INF00; %12f inf right-justified in width 12.
 */
#include "tc.h"
#include <stdio.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile unsigned long long b_pinf = 0x7ff0000000000000ULL, b_ninf = 0xfff0000000000000ULL, b_qnan = 0x7ff8000000000000ULL, b_ind = 0xfff8000000000000ULL, b_snan = 0x7ff4000000000000ULL, b_nsnan = 0xfff4000000000000ULL;
#define PINF D(b_pinf)
#define NINF D(b_ninf)
#define QNAN D(b_qnan)
#define IND D(b_ind)
#define SNAN D(b_snan)

TC_CASE(snan_f) { char b[64]; sprintf(b, "%f", SNAN); CHECK_STR(b, "1.#SNAN0"); }
TC_CASE(g_qnan) { char b[64]; sprintf(b, "%g", QNAN); CHECK_STR(b, "1.#QNAN"); }
TC_CASE(g_indefinite) { char b[64]; sprintf(b, "%g", IND); CHECK_STR(b, "-1.#IND"); }
TC_CASE(plus_inf) { char b[64]; sprintf(b, "%+f", PINF); CHECK_STR(b, "+1.#INF00"); }
TC_CASE(width_inf) { char b[64]; sprintf(b, "%12f", PINF); CHECK_STR(b, "    1.#INF00"); }
int main(void) { tc_begin(); TC_RUN(snan_f); TC_RUN(g_qnan); TC_RUN(g_indefinite); TC_RUN(plus_inf); TC_RUN(width_inf); return tc_end(); }
