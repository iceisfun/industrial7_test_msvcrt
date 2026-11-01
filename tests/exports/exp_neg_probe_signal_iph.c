/* TC
 * id:    exp_neg_probe_signal_iph
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: signal raise _errno
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs signal, raise, Parameter Validation
 * note:  Probing: signal(9999)/raise(9999) invoke the invalid-parameter handler (once each) and raise sets errno EINVAL.
 * note:  OBSERVED (Win10 22H2 19045; raw "tc_iph_count == 1 got=0" for signal): signal(9999,h) returns SIG_ERR without invoking the
 *        invalid-parameter handler; raise(9999) does (count 1, errno 22). Class A.
 */
#include "tc.h"

typedef void (__cdecl *sig_h)(int);
typedef sig_h (__cdecl *signal_t)(int, sig_h);
typedef int (__cdecl *raise_t)(int);
typedef int *(__cdecl *errno_t_)(void);
static void __cdecl h(int s) { (void)s; }

TC_CASE(signal_calls_handler)
{
    signal_t sg = (signal_t)tc_crt_proc("signal");
    CHECK(sg != NULL);
    if (!sg) return;
    tc_iph_count = 0;
    CHECK((intptr_t)sg(9999, h) == -1);
    CHECK_EQ_I(tc_iph_count, 0);   /* observed Win10 22H2: signal() with a bad signum fails with SIG_ERR but does NOT call the handler */
}
TC_CASE(raise_calls_handler_and_sets_errno)
{
    raise_t rs = (raise_t)tc_crt_proc("raise"); errno_t_ en = (errno_t_)tc_crt_proc("_errno");
    CHECK(rs && en);
    if (!(rs && en)) return;
    tc_iph_count = 0; *en() = 0;
    CHECK(rs(9999) != 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(*en(), 22);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(signal_calls_handler); TC_RUN(raise_calls_handler_and_sets_errno);
    return tc_end();
}
