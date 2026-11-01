/* TC
 * id:    io_neg_default_handler_close_fatal
 * area:  lowio
 * funcs: _close
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation; _close (invalid fd -> invalid parameter handler)
 * note:  Documented: _close(-1) with the default handler terminates the process (child must not return 77).
 *        PROBES the exact status: 0xC0000409 (fast fail) on ucrtbase.
 */
#include "tc.h"
#include <io.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("close", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); volatile int fd = -1; _close(fd); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
