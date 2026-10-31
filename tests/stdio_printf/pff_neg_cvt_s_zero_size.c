/* TC
 * id:    pff_neg_cvt_s_zero_size
 * area:  stdio_printf
 * funcs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ecvt_s, _fcvt_s: sizeInBytes of zero is an invalid parameter -> EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(ecvt_s_zero)
{
    char b[16]; int d = 7, s = 7; errno_t r; int i;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    for (i = 0; i < 16; i++) b[i] = 'Q';
    tc_iph_count = 0;
    r = _ecvt_s(b, 0, 123.456, 3, &d, &s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 16; i++) CHECK_EQ_I(b[i], 'Q');
}
TC_CASE(fcvt_s_zero)
{
    char b[16]; int d = 7, s = 7; errno_t r; int i;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    for (i = 0; i < 16; i++) b[i] = 'Q';
    tc_iph_count = 0;
    r = _fcvt_s(b, 0, 123.456, 3, &d, &s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 16; i++) CHECK_EQ_I(b[i], 'Q');
}

int main(void) { tc_begin(); TC_RUN(ecvt_s_zero); TC_RUN(fcvt_s_zero); return tc_end(); }
