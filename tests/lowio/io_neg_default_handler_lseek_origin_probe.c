/* TC
 * id:    io_neg_default_handler_lseek_origin_probe
 * area:  lowio
 * funcs: _lseek _open
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lseek (invalid origin: documented as invalid-parameter case)
 * note:  PROBES: _lseek(validfd, 0, 7) invokes the invalid-parameter handler; with the default handler the child
 *        terminates with 0xC0000409 (rather than returning -1/EINVAL and continuing, which would exit 77).
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>

TC_CASE(bad_origin)
{
    DWORD code = 0; int ran = tc_spawn_self("origin", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile int org = 7; int fd = _open("NUL", _O_RDWR | _O_BINARY);
        if (fd < 0) return 78;
        _lseek(fd, 0, org);
        return 77;
    }
    TC_RUN(bad_origin);
    return tc_end();
}
