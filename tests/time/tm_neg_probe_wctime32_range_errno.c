/* TC
 * id:    tm_neg_probe_wctime32_range_errno
 * area:  time
 * kind:  negative
 * funcs: _wctime32
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs ctime (return value only documented as NULL)
 * note:  PROBE: whether _wctime32 of a negative timer sets errno to EINVAL (expected: yes, via the localtime range check).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
static volatile __time32_t vt;
TC_CASE(minus_one) { __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); vt = -1; t = vt; errno = 0; r = _wctime32(&t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(int_min) { __time32_t t; wchar_t *r; _putenv("TZ=UTC0"); _tzset(); vt = INT_MIN; t = vt; errno = 0; r = _wctime32(&t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(int_min); return tc_end(); }
