/* TC
 * id:    hp_neg_aligned_msize_null_fatal
 * area:  heap
 * kind:  negative
 * funcs: _aligned_msize
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _aligned_msize: NULL memblock is an invalid parameter; default handler terminates the process
 * note:  Probes exact status 0xC0000409 (docs only promise termination). Runs in a child process.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

TC_CASE(null_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullmsize", &code, 20000);
    CHECK(ran);
    CHECK(code != 0 && code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    if (tc_begin(), tc_child_name()) {
        void *volatile p = NULL;
        (void)_aligned_msize(p, 16, 0);
        return 77;
    }
    TC_RUN(null_terminates);
    return tc_end();
}
