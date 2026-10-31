/* TC
 * id:    exp_neg_configthreadlocale_bad_flag
 * area:  exports
 * funcs:
 * procs: _configthreadlocale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _configthreadlocale: invalid argument -> invalid parameter handler, returns -1 (EINVAL)
 * note:  The state is never changed by this test (only an invalid flag and the query value 0 are passed).
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cfg_t)(int);
static cfg_t p_cfg;
static volatile int flag;

TC_CASE(flag_0x1234_minus1)
{
    int r;
    if (!p_cfg) TC_SKIP("_configthreadlocale not exported");
    tc_iph_install(); flag = 0x1234; errno = 0;
    r = p_cfg(flag);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_called)
{
    if (!p_cfg) TC_SKIP("_configthreadlocale not exported");
    tc_iph_install(); tc_iph_count = 0; flag = 0x1234;
    p_cfg(flag);
    CHECK(tc_iph_count > 0);
}
TC_CASE(valid_query_zero_ok)
{
    int r;
    if (!p_cfg) TC_SKIP("_configthreadlocale not exported");
    tc_iph_install(); tc_iph_count = 0; flag = 0;
    r = p_cfg(flag);
    CHECK(r != -1);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_cfg = (cfg_t)tc_crt_proc("_configthreadlocale");
    TC_RUN(flag_0x1234_minus1); TC_RUN(iph_called); TC_RUN(valid_query_zero_ok);
    return tc_end();
}
