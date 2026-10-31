/* TC
 * id:    gio_neg_filelength_nul_pipe_probe
 * area:  lowio
 * funcs: _filelength _filelengthi64 _open _pipe _close
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBES _filelength on non-disk handles: the NUL device yields 0; a pipe yields -1 (seek fails) or 0, and when
 *        it fails errno is set non-zero. The invalid-parameter handler is replaced and its count not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
TC_CASE(nul_device_zero)
{
    int fd = _open("NUL", _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_filelengthi64(fd), 0);
    _close(fd);
}
TC_CASE(pipe_fails_or_zero)
{
    int p[2] = {-1, -1}; long r; long long r64;
    CHECK_EQ_I(_pipe(p, 4096, _O_BINARY), 0);
    if (p[0] < 0) return;
    r = _filelength(p[0]); r64 = _filelengthi64(p[0]);
    CHECK(r == -1 || r == 0);
    CHECK(r64 == -1 || r64 == 0);
    _close(p[0]); _close(p[1]);
}
TC_CASE(errno_recorded_when_failed)
{
    int p[2] = {-1, -1};
    CHECK_EQ_I(_pipe(p, 4096, _O_BINARY), 0);
    if (p[0] < 0) return;
    errno = 0;
    if (_filelength(p[1]) == -1) CHECK(errno != 0);
    _close(p[0]); _close(p[1]);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(nul_device_zero); TC_RUN(pipe_fails_or_zero); TC_RUN(errno_recorded_when_failed);
    return tc_end();
}
