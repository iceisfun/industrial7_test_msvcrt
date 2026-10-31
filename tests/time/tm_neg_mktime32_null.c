/* TC
 * id:    tm_neg_mktime32_null
 * area:  time
 * kind:  negative
 * funcs: _mktime32 _mkgmtime32
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs mktime, _mkgmtime (NULL pointer is an invalid parameter: returns -1, errno EINVAL)
 * note:  msvcrt.dll excluded (invalid-parameter handler not assured).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
static struct tm *volatile vp;
TC_CASE(mktime32) { __time32_t r; errno = 0; tc_iph_count = 0; r = _mktime32((struct tm *)vp); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mkgmtime32) { __time32_t r; errno = 0; tc_iph_count = 0; r = _mkgmtime32((struct tm *)vp); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mktime32); TC_RUN(mkgmtime32); return tc_end(); }
