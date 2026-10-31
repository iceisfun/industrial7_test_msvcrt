/* TC
 * id:    prc_neg_popen_mode_tb_probe
 * area:  process
 * funcs: _popen _pclose
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _popen (modes: r/w with optional t/b)
 * note:  PROBE: contradictory / unknown mode strings "rtb", "rw+", "a", "R" are rejected with NULL, errno EINVAL and one
 *        invalid-parameter call. One case per string so a wrong guess does not mask the others. The controls "rt" and "rb"
 *        are documented-valid.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vmode;
static void bad(const char *m)
{
    FILE *f;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0; vmode = m;
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (f) _pclose(f);
}
static void good(const char *m)
{
    FILE *f; vmode = m;
    tc_iph_install();
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(_pclose(f), 0);
}
TC_CASE(mode_rtb)  { bad("rtb"); }
TC_CASE(mode_rwp)  { bad("rw+"); }
TC_CASE(mode_a)    { bad("a"); }
TC_CASE(mode_R)    { bad("R"); }
TC_CASE(ctl_rt)    { good("rt"); }
TC_CASE(ctl_rb)    { good("rb"); }
int main(void)
{
    tc_begin();
    TC_RUN(mode_rtb); TC_RUN(mode_rwp); TC_RUN(mode_a); TC_RUN(mode_R); TC_RUN(ctl_rt); TC_RUN(ctl_rb);
    return tc_end();
}
