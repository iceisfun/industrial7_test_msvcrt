/* TC
 * id:    ext_neg_raise_invalid_signum_msvcrt
 * area:  exit_signal
 * kind:  negative
 * funcs: raise
 * crt:   msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs raise
 * note:  errno is deliberately not asserted for msvcrt.dll (see ext_probe_neg_raise_invalid_errno_msvcrt).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
TC_CASE(invalid_numbers)
{
    static const int bad[] = {0,9,-1};
    size_t i;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        volatile int sn = bad[i];
        int r;
        r = raise(sn);
        CHECK(r != 0);
    }
}
int main(void) { tc_begin(); TC_RUN(invalid_numbers); return tc_end(); }
