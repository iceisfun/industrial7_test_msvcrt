/* TC
 * id:    ext_neg_signal_failure_leaves_handlers_unchanged
 * area:  exit_signal
 * funcs: signal raise
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs signal: invalid signal number => SIG_ERR, errno EINVAL, invalid parameter handler
 * note:  After a raise the handler is reset to SIG_DFL (MS docs signal), so state is verified by re-installing before raising.
 *        A failed signal(bad, h2) must not install h2 or disturb the handlers already installed for valid signals. OBSERVED on
 *        Win10 22H2 ucrtbase: signal(0 / -1 / 99) returns SIG_ERR with errno EINVAL but does NOT call the invalid-parameter
 *        handler (the old expectation of one call was wrong).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>

typedef void (__cdecl *sh_t)(int);
static volatile int c1, c2;
static void __cdecl h1(int s) { (void)s; c1++; }
static void __cdecl h2(int s) { (void)s; c2++; }
static const int valid[] = { SIGTERM, SIGINT, SIGABRT };

TC_CASE(failed_signal_no_effect)
{
    static const int bad[] = { 0, -1, 99 };
    size_t i;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    for (i = 0; i < 3; i++) CHECK(signal(valid[i], h1) == SIG_DFL);
    for (i = 0; i < 3; i++) {
        volatile int sn = bad[i]; int before = tc_iph_count; sh_t r;
        errno = 0;
        r = signal(sn, h2);
        CHECK(r == SIG_ERR);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, before);   /* observed: no handler call */
    }
    for (i = 0; i < 3; i++) {   /* h1 still installed: re-installing it returns h1 (no raise yet) */
        CHECK(signal(valid[i], h1) == h1);
    }
    for (i = 0; i < 3; i++) {
        c1 = c2 = 0;
        CHECK_EQ_I(raise(valid[i]), 0);
        CHECK_EQ_I(c1, 1);
        CHECK_EQ_I(c2, 0);
    }
}
TC_CASE(reset_after_dispatch)
{
    /* MS docs signal: before the handler runs, func is reset to SIG_DFL, so after the raises above the state is SIG_DFL. */
    size_t i;
    for (i = 0; i < 3; i++) CHECK(signal(valid[i], SIG_DFL) == SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(failed_signal_no_effect); TC_RUN(reset_after_dispatch); return tc_end(); }
