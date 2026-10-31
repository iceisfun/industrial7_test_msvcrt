/* TC
 * id:    err_neg_probe_strerror_extreme_errno_iph
 * area:  errno_param
 * kind:  negative
 * funcs: strerror _strerror _wcserror
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strerror (silent on errno and parameter validation for out-of-range numbers)
 * note:  PROBE: strerror/_wcserror with out-of-range numbers (INT_MIN, INT_MAX, -1) neither change errno nor invoke the
 *        invalid-parameter handler (strerror clamps unknown values to the "Unknown error" entry).
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static volatile int v_e;

TC_CASE(strerror_errno_iph)
{
    errno = 77; tc_iph_count = 0;
    v_e = INT_MIN; (void)strerror(v_e); v_e = INT_MAX; (void)strerror(v_e); v_e = -1; (void)strerror(v_e);
    CHECK_EQ_I(errno, 77); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wcserror_errno_iph)
{
    errno = 77; tc_iph_count = 0;
    v_e = INT_MIN; (void)_wcserror(v_e); v_e = INT_MAX; (void)_wcserror(v_e);
    CHECK_EQ_I(errno, 77); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(_strerror_errno_iph)
{
    errno = INT_MAX; tc_iph_count = 0; (void)_strerror(NULL);
    CHECK_EQ_I(errno, INT_MAX); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(strerror_errno_iph); TC_RUN(wcserror_errno_iph); TC_RUN(_strerror_errno_iph);
    return tc_end();
}
