/* TC
 * id:    ext_neg_signal_invalid_signum
 * area:  exit_signal
 * kind:  negative
 * funcs: signal _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs signal: invalid signal number => SIG_ERR, errno EINVAL, invalid parameter handler
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
static void __cdecl h(int s) { (void)s; }
TC_CASE(invalid_numbers)
{
    static const int bad[] = {0,1,3,5,7,9,10,12,13,14,16,20,23,-1,1000};
    size_t i;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        volatile int sn = bad[i];
        int before = tc_iph_count;
        void (__cdecl *r)(int);
        errno = 0;
        r = signal(sn, h);
        if (r != SIG_ERR) tc_fail_expr(__FILE__, __LINE__, "signal(bad) != SIG_ERR");
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, before + 1);
    }
}
TC_CASE(valid_still_works)
{
    void (__cdecl *r)(int);
    r = signal(SIGTERM, h);
    CHECK(r != SIG_ERR);
    r = signal(SIGTERM, SIG_DFL);
    CHECK(r == h);
}
int main(void) { tc_begin(); TC_RUN(invalid_numbers); TC_RUN(valid_still_works); return tc_end(); }
