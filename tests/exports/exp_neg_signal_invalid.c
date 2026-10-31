/* TC
 * id:    exp_neg_signal_invalid
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: signal raise _errno
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs signal: returns SIG_ERR and sets errno EINVAL for an invalid signal; MS docs raise: nonzero on failure
 * note:  Invalid-parameter handler is installed since both paths validate the argument.
 */
#include "tc.h"

typedef void (__cdecl *sig_h)(int);
typedef sig_h (__cdecl *signal_t)(int, sig_h);
typedef int (__cdecl *raise_t)(int);
typedef int *(__cdecl *errno_t_)(void);
static void __cdecl h(int s) { (void)s; }

TC_CASE(signal_bad_number)
{
    signal_t sg = (signal_t)tc_crt_proc("signal"); errno_t_ en = (errno_t_)tc_crt_proc("_errno");
    sig_h r;
    CHECK(sg && en);
    if (!(sg && en)) return;
    *en() = 0;
    r = sg(9999, h);
    CHECK((intptr_t)r == -1);   /* SIG_ERR */
    CHECK_EQ_I(*en(), 22);      /* EINVAL */
}
TC_CASE(raise_bad_number)
{
    raise_t rs = (raise_t)tc_crt_proc("raise");
    CHECK(rs != NULL);
    if (rs) CHECK(rs(9999) != 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(signal_bad_number); TC_RUN(raise_bad_number);
    return tc_end();
}
