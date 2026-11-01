/* TC
 * id:    ext_neg_signal_invalid_signum
 * area:  exit_signal
 * kind:  negative
 * funcs: signal _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs signal: invalid signal number => SIG_ERR, errno EINVAL, invalid parameter handler
 * note:  MS docs signal: invalid signal number => SIG_ERR, errno EINVAL, invalid parameter handler. OBSERVED on Win10 22H2
 *        ucrtbase: all 15 numbers return SIG_ERR but the handler is never invoked, and errno is 0 (not EINVAL) for 1, 3, 13 and
 *        16
 *        (class B: deviates from C11 7.14.1.1p8, which stores a positive value in errno on a signal() failure).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
static void __cdecl h(int s) { (void)s; }
/* OBSERVED on Win10 22H2 ucrtbase (two runs): every unsupported number returns SIG_ERR and NEVER calls the invalid-parameter
 * handler; errno is EINVAL for most, but stays 0 for 1, 3, 13 and 16. */
TC_CASE(invalid_numbers)
{
    static const struct { int sn, err; } bad[] = { {0,EINVAL},{1,0},{3,0},{5,EINVAL},{7,EINVAL},{9,EINVAL},{10,EINVAL},{12,EINVAL},{13,0},{14,EINVAL},
                                                   {16,0},{20,EINVAL},{23,EINVAL},{-1,EINVAL},{1000,EINVAL} };
    size_t i;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        volatile int sn = bad[i].sn;
        int before = tc_iph_count;
        void (__cdecl *r)(int);
        errno = 0;
        r = signal(sn, h);
        if (r != SIG_ERR) tc_fail_expr(__FILE__, __LINE__, "signal(bad) != SIG_ERR");
        CHECK_EQ_I(errno, bad[i].err);
        CHECK_EQ_I(tc_iph_count, before);
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
