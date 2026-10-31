/* TC
 * id:    ext_neg_raise_invalid_signum
 * area:  exit_signal
 * kind:  negative
 * funcs: raise _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs raise: invalid signal => nonzero, EINVAL, invalid parameter handler
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
TC_CASE(invalid_numbers)
{
    static const int bad[] = {0,1,3,9,23,-1,1000};
    size_t i;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        volatile int sn = bad[i];
        int before = tc_iph_count, r;
        errno = 0;
        r = raise(sn);
        CHECK(r != 0);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, before + 1);
    }
}
int main(void) { tc_begin(); TC_RUN(invalid_numbers); return tc_end(); }
