/* TC
 * id:    pfu_sprintf_s_nan_sign_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   UCRT behaviour from memory (not documented)
 * note:  PROBING exactly: spelling of NaNs with the sign bit set in UCRT sprintf_s. Expectation: the x86 "real indefinite" 0xFFF8000000000000 prints "-nan(ind)"; a negative quiet NaN with a payload (0xFFF8000000000001) prints "-nan"; a signalling NaN (0x7FF0000000000001) prints "nan(snan)".
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned long long ind = 0xFFF8000000000000ULL, nq = 0xFFF8000000000001ULL, sn = 0x7FF0000000000001ULL;
static double D(volatile unsigned long long *p) { union { unsigned long long u; double d; } x; x.u = *p; return x.d; }

TC_CASE(indefinite) { char b[32]; sprintf_s(b, 32, "%f", D(&ind)); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(negative_qnan_payload) { char b[32]; sprintf_s(b, 32, "%f", D(&nq)); CHECK_STR(b, "-nan"); }
TC_CASE(signalling)
{
#ifndef _WIN64
    TC_SKIP("x87 double return quiets the signalling NaN");
#endif
    { char b[32]; sprintf_s(b, 32, "%f", D(&sn)); CHECK_STR(b, "nan(snan)"); }
}

int main(void) { tc_begin(); TC_RUN(indefinite); TC_RUN(negative_qnan_payload); TC_RUN(signalling); return tc_end(); }
