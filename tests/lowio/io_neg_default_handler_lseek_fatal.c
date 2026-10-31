/* TC
 * id:    io_neg_default_handler_lseek_fatal
 * area:  lowio
 * funcs: _lseek
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lseek, Parameter validation (bad fd -> invalid parameter handler)
 * note:  Documented: with the default handler the child terminates (never return 77, exit status nonzero).
 *        PROBES the exact status 0xC0000409.
 */
#include "tc.h"
#include <io.h>

TC_CASE(bad_fd)
{
    DWORD code = 0; int ran = tc_spawn_self("badfd", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int bad = -1;
        _lseek(bad, 0, 0);
        return 77;
    }
    TC_RUN(bad_fd);
    return tc_end();
}
