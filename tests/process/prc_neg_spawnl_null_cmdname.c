/* TC
 * id:    prc_neg_spawnl_null_cmdname
 * area:  process
 * funcs: _spawnl
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn: NULL cmdname invokes the invalid-parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(null_cmdname)
{
    volatile int m = _P_WAIT; const char *volatile nul = NULL; intptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _spawnl(m, nul, "x.exe", NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(null_cmdname);
    return tc_end();
}
