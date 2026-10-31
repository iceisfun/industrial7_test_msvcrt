/* TC
 * id:    tm_neg_ctime_probe_range_errno
 * area:  time
 * kind:  negative
 * funcs: _ctime64 _ctime32
 * procs: _ctime64_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs ctime, ctime_s (range failure)
 * note:  PROBE: (1) errno == EINVAL after _ctime64/_ctime32 return NULL for an out-of-range time (not documented on the ctime page);
 *        (2) _ctime64_s with a valid 26-byte buffer and out-of-range time leaves buffer[0] == 0 (empty string).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>

TC_DECL_PROC(int, _ctime64_s, (char *, size_t, const __time64_t *));
static volatile __time64_t vt;
static volatile __time32_t vt32;

TC_CASE(ctime64_neg_errno)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); errno = 0;
    vt = -1; t = vt; r = _ctime64(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(ctime64_big_errno)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); errno = 0;
    vt = 32535244800LL; t = vt; r = _ctime64(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(ctime32_neg_errno)
{
    __time32_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); errno = 0;
    vt32 = -1; t = vt32; r = _ctime32(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(ctime64_s_resets_buffer)
{
    char b[26]; __time64_t t;
    if (!tc_p__ctime64_s) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); b[0] = 'x'; vt = -1; t = vt;
    CHECK_EQ_I(tc_p__ctime64_s(b, 26, &t), EINVAL);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_ctime64_s);
    TC_RUN(ctime64_neg_errno); TC_RUN(ctime64_big_errno); TC_RUN(ctime32_neg_errno); TC_RUN(ctime64_s_resets_buffer);
    return tc_end();
}
