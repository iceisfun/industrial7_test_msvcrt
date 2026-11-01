/* TC
 * id:    ext_neg_signal_invalid_default_iph_fatal
 * area:  exit_signal
 * kind:  negative
 * funcs: signal
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs signal / Parameter validation: default handler terminates the process
 * note:  Asserts only that the child terminates abnormally (not exit 0, not the 77 survival marker); exact status not asserted.
 *        Win10 22H2 19045 (observed, ucrtbase; TC_AT_LEAST(140) only): signal() with an invalid number (0, 1000, -1, 5, 7, 9, 12, 255) does NOT reach the invalid parameter
 *        handler even with the CRT default handler restored: it returns SIG_ERR with errno EINVAL and the process continues (the MS docs describe the handler call;
 *        Windows deviates). Analysis of ucrtbase : signal() -> signal_failed(), which only sets errno = EINVAL and
 *        returns -1, no _invalid_parameter call. Case child_terminates keeps its name and asserts exit 77 (= survived with SIG_ERR/EINVAL); in-process case
 *        no_handler_call asserts the handler is not called.
 *        [unverified] on msvcr80..120: the original abnormal-termination expectation is kept there.
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
static void __cdecl h(int s) { (void)s; }
TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badsig", &code, 20000);
    CHECK(ran);
#if TC_AT_LEAST(140)
    CHECK_EQ_U(code, 77);   /* observed: the child survives; signal(0) returned SIG_ERR with errno EINVAL (child maps any other outcome to 78/79) */
#else
    CHECK(code != 0);
    CHECK(code != 77);
#endif
}
#if TC_AT_LEAST(140)
TC_CASE(no_handler_call)
{
    volatile int sn = 0; void (__cdecl *r)(int);
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = signal(sn, h);
    CHECK(r == SIG_ERR);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 0);   /* observed: the invalid parameter handler is not called */
}
#endif
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int sn = 0; void (__cdecl *r)(int);
        tc_iph_default();
#if TC_AT_LEAST(140)
        errno = 0; r = signal(sn, h); if (r != SIG_ERR) return 78; if (errno != EINVAL) return 79; return 77;
#else
        r = signal(sn, h); (void)r; return 77;
#endif
    }
    TC_RUN(child_terminates);
#if TC_AT_LEAST(140)
    TC_RUN(no_handler_call);
#endif
    return tc_end();
}
