/* TC
 * id:    prc_neg_popen_mode_tb_probe
 * area:  process
 * funcs: _popen _pclose
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _popen (modes: r/w with optional t/b)
 * note:  OBSERVED on Win10 22H2 (19045): _popen modes "rtb" and "rw+" are ACCEPTED (non-NULL stream, _pclose == 0); only the
 *        unknown first characters "a" and "R" are rejected with NULL, EINVAL and one handler call. The earlier expectation that
 *        contradictory/extra mode characters are rejected was wrong. Stable.
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
static void accepted(const char *m)
{
    FILE *f; vmode = m;
    tc_iph_install();
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f != NULL);     /* observed: accepted */
    if (f) CHECK_EQ_I(_pclose(f), 0);
}
static void good(const char *m)
{
    FILE *f; vmode = m;
    tc_iph_install();
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(_pclose(f), 0);
}
TC_CASE(mode_rtb_accepted)  { accepted("rtb"); }
TC_CASE(mode_rwp_accepted)  { accepted("rw+"); }
TC_CASE(mode_a)    { bad("a"); }
TC_CASE(mode_R)    { bad("R"); }
TC_CASE(ctl_rt)    { good("rt"); }
TC_CASE(ctl_rb)    { good("rb"); }
int main(void)
{
    tc_begin();
    TC_RUN(mode_rtb_accepted); TC_RUN(mode_rwp_accepted); TC_RUN(mode_a); TC_RUN(mode_R); TC_RUN(ctl_rt); TC_RUN(ctl_rb);
    return tc_end();
}
