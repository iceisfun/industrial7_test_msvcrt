/* TC
 * id:    tm_neg_gmtime32_negative
 * area:  time
 * funcs: _gmtime32
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime32
 * note:  Win10 19045 ucrtbase: _gmtime32(-1) succeeds (min accepted is -43200); the rejection cases are -43201 and INT_MIN.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time32_t v_t;

/* observed on Win10 ucrtbase: _gmtime32 accepts [-43200, INT_MAX]; -1 returns 1970-01-01 00:00:-1 (tm_sec == -1, outside C11 7.27.1 range 0-60:
 * deviates from C11 7.27.1), -43201 and INT_MIN are NULL/EINVAL */
TC_CASE(minus_one) { struct tm *r; v_t = -1; errno = 0; r = _gmtime32((const __time32_t *)&v_t);
#if TC_CRT_VER >= 140
    CHECK(r != NULL); CHECK_EQ_I(errno, 0);
    if (r) { CHECK_EQ_I(r->tm_year, 70); CHECK_EQ_I(r->tm_mon, 0); CHECK_EQ_I(r->tm_mday, 1); CHECK_EQ_I(r->tm_hour, 0); CHECK_EQ_I(r->tm_min, 0); CHECK_EQ_I(r->tm_sec, -1); }
#else
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
#endif
}
TC_CASE(below_min) { struct tm *r; v_t = -43201; errno = 0; r = _gmtime32((const __time32_t *)&v_t);
#if TC_CRT_VER >= 140
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
#else
    (void)r; TC_SKIP("ucrtbase-only observation");
#endif
}
TC_CASE(int_min) { struct tm *r; v_t = INT_MIN; errno = 0; r = _gmtime32((const __time32_t *)&v_t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_ptr) { struct tm *r; errno = 0; tc_iph_count = 0; r = _gmtime32(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(below_min); TC_RUN(int_min); TC_RUN(null_ptr); return tc_end(); }
