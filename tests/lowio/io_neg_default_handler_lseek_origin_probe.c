/* TC
 * id:    io_neg_default_handler_lseek_origin_probe
 * area:  lowio
 * funcs: _lseek _open
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _lseek (invalid origin: documented as invalid-parameter case)
 * note:  OBSERVED on Win10 22H2 ucrtbase (default invalid-parameter handler restored in the child): _lseek(validfd, 0, 7) does
 *        NOT reach the invalid-parameter handler; it returns -1 with errno EINVAL and the child continues (exit 77; 78 would
 *        mean a different return/errno). The old fast-fail expectation was wrong.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>

TC_CASE(bad_origin)
{
    DWORD code = 0; int ran = tc_spawn_self("origin", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);   /* observed: the child returns normally, the handler is not reached */
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        tc_iph_default();
        volatile int org = 7; int fd = _open("NUL", _O_RDWR | _O_BINARY);
        if (fd < 0) return 78;
        errno = 0;
        if (_lseek(fd, 0, org) != -1 || errno != EINVAL) return 78;
        return 77;
    }
    TC_RUN(bad_origin);
    return tc_end();
}
