/* TC
 * id:    io_neg_pipe_write_closed_reader
 * area:  lowio
 * funcs: _pipe _write _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _write: returns -1 on error; errno mapping of ERROR_NO_DATA / ERROR_BROKEN_PIPE
 * note:  Probes: writing to a pipe whose read end is closed returns -1 (near certain) and errno is EPIPE (ERROR_NO_DATA / ERROR_BROKEN_PIPE both map to EPIPE in the CRT errno table). Each assertion is its own case.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_neg_pipe_write_closed_reader_" TC_CRT_DLL ".tmp"

static int setup(int f[2])
{
    f[0] = f[1] = -1;
    if (_pipe(f, 4096, _O_BINARY) != 0) return 0;
    _close(f[0]);
    return 1;
}
TC_CASE(write_fails)
{
    int f[2];
    if (tc_iph_install()) {}
    CHECK(setup(f));
    if (f[1] < 0) return;
    errno = 0;
    CHECK_EQ_I(_write(f[1], "abcd", 4), -1);
    _close(f[1]);
}
TC_CASE(errno_epipe)
{
    int f[2];
    CHECK(setup(f));
    if (f[1] < 0) return;
    errno = 0;
    _write(f[1], "abcd", 4);
    CHECK_EQ_I(errno, EPIPE);
    _close(f[1]);
}

int main(void)
{
    tc_begin();
    TC_RUN(write_fails);
    TC_RUN(errno_epipe);
    return tc_end();
}
