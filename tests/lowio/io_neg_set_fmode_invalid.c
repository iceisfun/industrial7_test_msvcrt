/* TC
 * id:    io_neg_set_fmode_invalid
 * area:  lowio
 * funcs: _get_fmode _set_fmode _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode / _get_fmode: invalid mode or NULL -> EINVAL and invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_neg_set_fmode_invalid_" TC_CRT_DLL ".tmp"

static int cur(void) { int m = -99; _get_fmode(&m); return m; }
TC_CASE(garbage)
{
    int before, r; CHECK(tc_iph_install());
    before = cur(); tc_iph_count = 0; errno = 0;
    r = _set_fmode(0x1234);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(cur(), before);
}
TC_CASE(append)
{
    int before, r; CHECK(tc_iph_install());
    before = cur(); tc_iph_count = 0; errno = 0;
    r = _set_fmode(_O_APPEND);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(cur(), before);
}
TC_CASE(negative)
{
    int before, r; CHECK(tc_iph_install());
    before = cur(); tc_iph_count = 0;
    r = _set_fmode(-1);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(cur(), before);
}
TC_CASE(get_null)
{
    int r; CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _get_fmode(NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(garbage);
    TC_RUN(append);
    TC_RUN(negative);
    TC_RUN(get_null);
    return tc_end();
}
