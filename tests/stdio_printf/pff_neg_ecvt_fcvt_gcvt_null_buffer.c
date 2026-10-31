/* TC
 * id:    pff_neg_ecvt_fcvt_gcvt_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: _ecvt_s _fcvt_s _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _ecvt_s/_fcvt_s/_gcvt_s: NULL buffer or sizeInBytes==0 -> EINVAL, invalid parameter handler invoked
 * note:  The errno value is asserted too (secure CRT functions set errno to the returned code). msvcr80 excluded:
 *        its import library lacks the _s functions.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(ecvt_null_buf)  { int d = 0, s = 0; char * volatile b = NULL; errno_t r; tc_iph_count = 0; errno = 0;
    r = _ecvt_s(b, 20, 1.0, 5, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fcvt_size_zero) { int d = 0, s = 0; char b[8] = "sent"; volatile size_t n = 0; errno_t r; tc_iph_count = 0; errno = 0;
    r = _fcvt_s(b, n, 1.0, 2, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_STR(b, "sent"); }
TC_CASE(gcvt_null_buf)  { char * volatile b = NULL; errno_t r; tc_iph_count = 0; errno = 0;
    r = _gcvt_s(b, 20, 1.0, 5); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fcvt_null_buf)  { int d = 0, s = 0; char * volatile b = NULL; errno_t r; tc_iph_count = 0; errno = 0;
    r = _fcvt_s(b, 20, 1.0, 2, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(ecvt_size_zero) { int d = 0, s = 0; char b[8] = "sent"; volatile size_t n = 0; errno_t r; tc_iph_count = 0; errno = 0;
    r = _ecvt_s(b, n, 1.0, 2, &d, &s); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_STR(b, "sent"); }
TC_CASE(gcvt_size_zero) { char b[8] = "sent"; volatile size_t n = 0; errno_t r; tc_iph_count = 0; errno = 0;
    r = _gcvt_s(b, n, 1.0, 2); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_STR(b, "sent"); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(ecvt_null_buf); TC_RUN(fcvt_null_buf); TC_RUN(gcvt_null_buf); TC_RUN(ecvt_size_zero); TC_RUN(fcvt_size_zero); TC_RUN(gcvt_size_zero);
    return tc_end();
}
