/* TC
 * id:    dir_neg_getcwd_negative_maxlen_probe
 * area:  dir
 * kind:  negative
 * funcs: _getcwd
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _getcwd (maxlen <= 0 is documented as an invalid parameter only for zero with a buffer)
 * note:  Probes _getcwd(buf, -1) with a valid MAX_PATH buffer: expect NULL, errno EINVAL, invalid parameter handler once.
 *        (maxlen is a signed int in the CRT, so -1 is rejected rather than treated as SIZE_MAX.)
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static volatile int v_neg = -1;

TC_CASE(minus_one)
{
    char buf[MAX_PATH]; char *p;
    buf[0] = 'x';
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    p = _getcwd(buf, v_neg);
    CHECK(p == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(minus_one);
    return tc_end();
}
