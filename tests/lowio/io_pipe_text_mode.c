/* TC
 * id:    io_pipe_text_mode
 * area:  lowio
 * funcs: _pipe _read _write _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _pipe (textmode), _write (LF -> CRLF), _read (CRLF -> LF)
 * note:  Probes translation on both ends of an _O_TEXT pipe: _write returns 2 for "a\n" (translation does not affect the return value, per _write docs) and _read returns the LF-only form.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_pipe_text_mode_" TC_CRT_DLL ".tmp"

TC_CASE(text_round_trip)
{
    int f[2] = { -1, -1 }; char b[16] = { 0 };
    CHECK_EQ_I(_pipe(f, 4096, _O_TEXT), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "a\n", 2), 2);
    CHECK_EQ_I(_read(f[0], b, 16), 2);
    CHECK_MEM(b, "a\n", 2);
    _close(f[0]); _close(f[1]);
}
TC_CASE(binary_reader_sees_crlf)
{
    int f[2] = { -1, -1 }; char b[16] = { 0 };
    CHECK_EQ_I(_pipe(f, 4096, _O_TEXT), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "a\n", 2), 2);
    _setmode(f[0], _O_BINARY);
    CHECK_EQ_I(_read(f[0], b, 16), 3);
    CHECK_MEM(b, "a\r\n", 3);
    _close(f[0]); _close(f[1]);
}

int main(void)
{
    tc_begin();
    TC_RUN(text_round_trip);
    TC_RUN(binary_reader_sees_crlf);
    return tc_end();
}
