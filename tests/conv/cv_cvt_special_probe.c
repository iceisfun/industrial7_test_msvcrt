/* TC
 * id:    cv_cvt_special_probe
 * area:  conv
 * funcs: _ecvt _gcvt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _ecvt/_gcvt
 * note:  Probes text produced for inf/nan. Assumed: '1#INF' / '1#QNAN' from _ecvt; _gcvt '1.#INF' (legacy), 'inf' (ucrt). Record per CRT. NaN is built from the positive quiet-NaN bit pattern (inf-inf would give the sign-set 'indefinite' on x86).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(ecvt_inf) { int d, s; double inf = v_one / v_zero; char *r = _ecvt(inf, 5, &d, &s); CHECK_STR(r, "1#INF"); CHECK_EQ_I(s, 0); }
TC_CASE(ecvt_nan) { int d, s; unsigned long long nb = 0x7FF8000000000000ULL; double nan; memcpy(&nan, &nb, sizeof nan); char *r = _ecvt(nan, 6, &d, &s); CHECK_STR(r, "1#QNAN"); CHECK_EQ_I(s, 0); }
TC_CASE(gcvt_inf) { char b[32]; double inf = v_one / v_zero; _gcvt(inf, 5, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "inf");
#else
    CHECK_STR(b, "1.#INF");
#endif
}
TC_CASE(gcvt_ninf) { char b[32]; double inf = v_one / v_zero; _gcvt(-inf, 5, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "-inf");
#else
    CHECK_STR(b, "-1.#INF");
#endif
}

int main(void)
{
    tc_begin();
    TC_RUN(ecvt_inf); TC_RUN(ecvt_nan); TC_RUN(gcvt_inf); TC_RUN(gcvt_ninf);
    return tc_end();
}
