/* TC
 * id:    gio_neg_tell_pipe_probe
 * area:  lowio
 * funcs: _tell _telli64 _pipe _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none (position of a pipe is undefined)
 * note:  PROBES that _tell/_telli64 on a pipe descriptor return -1 (SetFilePointer fails on pipes) and errno is one of
 *        EBADF / ESPIPE / EINVAL. The default handler is not expected to fire for a valid fd, but is replaced anyway.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

TC_CASE(tell_pipe_fails)
{
    int p[2] = {-1, -1};
    CHECK_EQ_I(_pipe(p, 4096, _O_BINARY), 0);
    if (p[0] < 0) return;
    errno = 0;
    CHECK_EQ_I(_tell(p[0]), -1);
    _close(p[0]); _close(p[1]);
}
TC_CASE(telli64_pipe_fails)
{
    int p[2] = {-1, -1};
    CHECK_EQ_I(_pipe(p, 4096, _O_BINARY), 0);
    if (p[0] < 0) return;
    errno = 0;
    CHECK_EQ_I(_telli64(p[1]), -1);
    _close(p[0]); _close(p[1]);
}
TC_CASE(errno_in_set)
{
    int p[2] = {-1, -1}, e;
    CHECK_EQ_I(_pipe(p, 4096, _O_BINARY), 0);
    if (p[0] < 0) return;
    errno = 0; _tell(p[0]); e = errno;
    CHECK(e == EBADF || e == ESPIPE || e == EINVAL);
    _close(p[0]); _close(p[1]);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(tell_pipe_fails); TC_RUN(telli64_pipe_fails); TC_RUN(errno_in_set);
    return tc_end();
}
