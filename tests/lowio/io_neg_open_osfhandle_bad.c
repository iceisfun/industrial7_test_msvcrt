/* TC
 * id:    io_neg_open_osfhandle_bad
 * area:  lowio
 * funcs: _open_osfhandle _open _close
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open_osfhandle ("returns -1 if an error occurs")
 * note:  PROBE. Returns -1 is documented; the errno value (EBADF for INVALID_HANDLE_VALUE) is the guess being probed,
 *        as is that no descriptor slot is consumed by the failure.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>

TC_CASE(invalid_handle_value)
{
    volatile intptr_t h = (intptr_t)INVALID_HANDLE_VALUE; int fd;
    tc_iph_install();
    errno = 0;
    fd = _open_osfhandle(h, _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EBADF);
    if (fd >= 0) _close(fd);
}
TC_CASE(no_fd_consumed)
{
    int a, b, x;
    a = _open("NUL", _O_RDONLY); CHECK(a >= 0);
    if (a >= 0) _close(a);
    tc_iph_install();
    x = _open_osfhandle((intptr_t)INVALID_HANDLE_VALUE, _O_RDONLY);
    CHECK_EQ_I(x, -1);
    b = _open("NUL", _O_RDONLY);
    CHECK_EQ_I(b, a);
    if (b >= 0) _close(b);
}
int main(void) { tc_begin(); TC_RUN(invalid_handle_value); TC_RUN(no_fd_consumed); return tc_end(); }
