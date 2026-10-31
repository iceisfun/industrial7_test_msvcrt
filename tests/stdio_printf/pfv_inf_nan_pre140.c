/* TC
 * id:    pfv_inf_nan_pre140
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   well-known pre-2015 MS CRT output (1.#INF, 1.#QNAN, 1.#IND)
 * note:  Counterpart of pfv_inf_nan_ucrt.
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

TC_CASE(inf_f) { char b[64]; sprintf(b, "%f", PINF); CHECK_STR(b, "1.#INF00"); }
TC_CASE(neg_inf_f) { char b[64]; sprintf(b, "%f", NINF); CHECK_STR(b, "-1.#INF00"); }
TC_CASE(inf_g) { char b[64]; sprintf(b, "%g", PINF); CHECK_STR(b, "1.#INF"); }
TC_CASE(qnan_f) { char b[64]; sprintf(b, "%f", QNAN); CHECK_STR(b, "1.#QNAN0"); }
TC_CASE(indefinite_f) { char b[64]; sprintf(b, "%f", IND); CHECK_STR(b, "-1.#IND00"); }
TC_CASE(inf_e) { char b[64]; sprintf(b, "%e", PINF); CHECK_STR(b, "1.#INF00e+000"); }
int main(void) { tc_begin(); TC_RUN(inf_f); TC_RUN(neg_inf_f); TC_RUN(inf_g); TC_RUN(qnan_f); TC_RUN(indefinite_f); TC_RUN(inf_e); return tc_end(); }
