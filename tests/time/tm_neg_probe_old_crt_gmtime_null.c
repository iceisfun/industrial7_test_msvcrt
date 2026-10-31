/* TC
 * id:    tm_neg_probe_old_crt_gmtime_null
 * area:  time
 * kind:  negative
 * funcs: _gmtime64 _localtime64
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _gmtime64, _localtime64 (NULL argument)
 * note:  msvcrt.dll excluded: NULL handling there is unknown and may crash. Probes NULL arguments on pre-UCRT runtimes: _gmtime64/_localtime64 return NULL with errno EINVAL. The invalid
 *        parameter handler is installed where the runtime supports it (msvcr100+); how often it fires is not asserted.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_CASE(gmtime64)    { struct tm *r; errno = 0; r = _gmtime64(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(localtime64) { struct tm *r; errno = 0; r = _localtime64(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin(); if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid-parameter handler API\n"); return tc_end(); }
    TC_RUN(gmtime64); TC_RUN(localtime64);
    return tc_end();
}
