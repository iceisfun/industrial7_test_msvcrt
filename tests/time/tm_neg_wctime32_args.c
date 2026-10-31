/* TC
 * id:    tm_neg_wctime32_args
 * area:  time
 * kind:  negative
 * funcs: _wctime32
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ctime, _ctime32, _wctime32 (NULL for time before midnight Jan 1 1970 UTC; NULL timer is an invalid parameter)
 * note:  errno for the negative-timer cases is asserted in tm_neg_probe_wctime32_range_errno, not here.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
static volatile __time32_t vt;
TC_CASE(null_timer) { const __time32_t *volatile p = NULL; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0; r = _wctime32((const __time32_t *)p); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(minus_one) { __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); vt = -1; t = vt; r = _wctime32(&t); CHECK(r == NULL); }
TC_CASE(int_min) { __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); vt = INT_MIN; t = vt; r = _wctime32(&t); CHECK(r == NULL); }
TC_CASE(zero_ok)
{
    __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); vt = 0; t = vt; r = _wctime32(&t);
    CHECK(r != NULL);
    if (r) { /* day-of-month padding differs across CRT generations: compare around it */
        wchar_t pre[] = L"Thu Jan "; wchar_t post[] = L" 00:00:00 1970\n"; int i;
        for (i = 0; i < 8; i++) CHECK_EQ_I(r[i], pre[i]);
        for (i = 0; post[i]; i++) CHECK_EQ_I(r[10 + i], post[i]);
        CHECK_EQ_I(r[25], 0);
    }
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_timer); TC_RUN(minus_one); TC_RUN(int_min); TC_RUN(zero_ok); return tc_end(); }
