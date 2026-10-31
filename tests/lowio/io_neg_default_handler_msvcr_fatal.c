/* TC
 * id:    io_neg_default_handler_msvcr_fatal
 * area:  lowio
 * funcs: _read
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation; _read (invalid fd)
 * note:  Documented: default handler terminates the child (not 77, nonzero). PROBES the exact status of the VS2005-2013
 *        CRTs: one of 0xC0000409 (fast fail), 0xC0000417 (invalid CRT parameter) or 0xC000000D (invalid parameter).
 */
#include "tc.h"
#include <io.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("read", &code, 30000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK(code == 0xC0000409u || code == 0xC0000417u || code == 0xC000000Du);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[4]; volatile int fd = -1; _read(fd, b, 1); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
