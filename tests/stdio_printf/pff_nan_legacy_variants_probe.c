/* TC
 * id:    pff_nan_legacy_variants_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   pre-UCRT CRT printf output (1.#QNAN, 1.#SNAN, 1.#IND)
 * note:  probed: legacy CRT distinguishes 1.#IND, 1.#QNAN, 1.#SNAN by payload bits
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

TC_CASE(snan) { volatile double d = dfb(0x7FF4000000000000ULL); sprintf(b, "%g", d); CHECK_STR(b, "1.#SNAN"); }
TC_CASE(neg_qnan_payload) { volatile double d = dfb(0xFFF8000000000001ULL); sprintf(b, "%g", d); CHECK_STR(b, "-1.#QNAN"); }
TC_CASE(neg_snan) { volatile double d = dfb(0xFFF4000000000000ULL); sprintf(b, "%g", d); CHECK_STR(b, "-1.#SNAN"); }
int main(void) { tc_begin(); TC_RUN(snan); TC_RUN(neg_qnan_payload); TC_RUN(neg_snan); return tc_end(); }
