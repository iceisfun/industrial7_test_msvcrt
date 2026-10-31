/* TC
 * id:    ext_neg_signal_invalid_signum_msvcrt
 * area:  exit_signal
 * kind:  negative
 * funcs: signal
 * crt:   msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs signal
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
static void __cdecl h(int s) { (void)s; }
TC_CASE(invalid_numbers)
{
    static const int bad[] = {0,9,23,-1};
    size_t i;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        volatile int sn = bad[i];
        void (__cdecl *r)(int);
        errno = 0;
        r = signal(sn, h);
        if (r != SIG_ERR) tc_fail_expr(__FILE__, __LINE__, "signal(bad) != SIG_ERR");
        CHECK_EQ_I(errno, EINVAL);
    }
}
int main(void) { tc_begin(); TC_RUN(invalid_numbers); return tc_end(); }
