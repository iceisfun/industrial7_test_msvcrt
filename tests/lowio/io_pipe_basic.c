/* TC
 * id:    io_pipe_basic
 * area:  lowio
 * funcs: _pipe _read _write _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _pipe
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_pipe_basic_" TC_CRT_DLL ".tmp"

TC_CASE(distinct_fds)
{
    int f[2] = { -1, -1 };
    CHECK_EQ_I(_pipe(f, 4096, _O_BINARY), 0);
    CHECK(f[0] >= 3); CHECK(f[1] >= 3); CHECK(f[0] != f[1]);
    _close(f[0]); _close(f[1]);
}
TC_CASE(round_trip)
{
    int f[2] = { -1, -1 }; char b[16] = { 0 };
    CHECK_EQ_I(_pipe(f, 4096, _O_BINARY), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "abcd", 4), 4);
    CHECK_EQ_I(_read(f[0], b, 4), 4);
    CHECK_MEM(b, "abcd", 4);
    _close(f[0]); _close(f[1]);
}
TC_CASE(eof_after_close)
{
    int f[2] = { -1, -1 }; char b[16];
    CHECK_EQ_I(_pipe(f, 4096, _O_BINARY), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "xy", 2), 2);
    CHECK_EQ_I(_close(f[1]), 0);
    CHECK_EQ_I(_read(f[0], b, 16), 2);
    CHECK_EQ_I(_read(f[0], b, 16), 0);
    _close(f[0]);
}
TC_CASE(binary_keeps_crlf)
{
    int f[2] = { -1, -1 }; char b[16] = { 0 };
    CHECK_EQ_I(_pipe(f, 4096, _O_BINARY), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "a\r\nb\n", 5), 5);
    CHECK_EQ_I(_read(f[0], b, 16), 5);
    CHECK_MEM(b, "a\r\nb\n", 5);
    _close(f[0]); _close(f[1]);
}

int main(void)
{
    tc_begin();
    TC_RUN(distinct_fds);
    TC_RUN(round_trip);
    TC_RUN(eof_after_close);
    TC_RUN(binary_keeps_crlf);
    return tc_end();
}
