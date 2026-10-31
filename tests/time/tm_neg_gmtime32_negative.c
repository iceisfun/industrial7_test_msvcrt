/* TC
 * id:    tm_neg_gmtime32_negative
 * area:  time
 * funcs: _gmtime32
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime32
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time32_t v_t;

TC_CASE(minus_one) { struct tm *r; v_t = -1; errno = 0; r = _gmtime32((const __time32_t *)&v_t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(int_min) { struct tm *r; v_t = INT_MIN; errno = 0; r = _gmtime32((const __time32_t *)&v_t); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_ptr) { struct tm *r; errno = 0; tc_iph_count = 0; r = _gmtime32(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(int_min); TC_RUN(null_ptr); return tc_end(); }
