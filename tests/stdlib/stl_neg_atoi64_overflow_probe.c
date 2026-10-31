/* TC
 * id:    stl_neg_atoi64_overflow_probe
 * area:  stdlib
 * funcs: _atoi64
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _atoi64 (overflow result not defined)
 * note:  Probes (ucrtbase only): _atoi64 on out-of-range text saturates to INT64_MAX / INT64_MIN. errno ERANGE is a
 *        separate case. Older CRTs are recorded (printed as "TC INFO") but not asserted.
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

static void info(const char *what, long long v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(pos_overflow)
{
    long long v = _atoi64("9223372036854775808");
    info("pos", v);
#ifdef TC_UCRT
    CHECK_EQ_I(v, 9223372036854775807LL);
#endif
}
TC_CASE(neg_overflow)
{
    long long v = _atoi64("-9223372036854775809");
    info("neg", v);
#ifdef TC_UCRT
    CHECK_EQ_I(v, (-9223372036854775807LL - 1));
#endif
}
TC_CASE(errno_erange)
{
    long long v; errno = 0; v = _atoi64("99999999999999999999");
    info("big", v); info("errno", errno);
#ifdef TC_UCRT
    CHECK_EQ_I(errno, ERANGE);
#endif
}
int main(void) { tc_begin(); TC_RUN(pos_overflow); TC_RUN(neg_overflow); TC_RUN(errno_erange); return tc_end(); }
