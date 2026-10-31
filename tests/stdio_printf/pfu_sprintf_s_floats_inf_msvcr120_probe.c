/* TC
 * id:    pfu_sprintf_s_floats_inf_msvcr120_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field characters (UCRT adopted C99 inf/nan; pre-UCRT printed 1.#INF00 / 1.#QNAN0 / -1.#IND00)
 * note:  Probing exactly: the pre-UCRT (msvcr120) spelling of infinity and NaN in %f. Expectation (most likely): the
 *        legacy forms "1.#INF00", "-1.#INF00", "1.#QNAN0", "-1.#IND00" - NOT C99 "inf"/"nan". Each form is its own case.
 */
#include "tc.h"
#include <stdio.h>

static unsigned long long bits_pinf = 0x7ff0000000000000ULL, bits_ninf = 0xfff0000000000000ULL;
static unsigned long long bits_qnan = 0x7ff8000000000000ULL, bits_ind  = 0xfff8000000000000ULL;
static double mk(volatile unsigned long long *b) { union { unsigned long long u; double d; } x; x.u = *b; return x.d; }

TC_CASE(pos_inf)  { char b[64]; sprintf_s(b, sizeof b, "%f", mk(&bits_pinf)); CHECK_STR(b, "1.#INF00"); }
TC_CASE(neg_inf)  { char b[64]; sprintf_s(b, sizeof b, "%f", mk(&bits_ninf)); CHECK_STR(b, "-1.#INF00"); }
TC_CASE(qnan)     { char b[64]; sprintf_s(b, sizeof b, "%f", mk(&bits_qnan)); CHECK_STR(b, "1.#QNAN0"); }
TC_CASE(indefinite) { char b[64]; sprintf_s(b, sizeof b, "%f", mk(&bits_ind)); CHECK_STR(b, "-1.#IND00"); }

int main(void) { tc_begin(); TC_RUN(pos_inf); TC_RUN(neg_inf); TC_RUN(qnan); TC_RUN(indefinite); return tc_end(); }
