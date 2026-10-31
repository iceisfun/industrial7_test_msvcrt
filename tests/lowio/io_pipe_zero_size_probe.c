/* TC
 * id:    io_pipe_zero_size_probe
 * area:  lowio
 * funcs: _pipe _read _write _close
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _pipe (psize); Win32 CreatePipe (nSize 0 = default buffer)
 * note:  Probes: _pipe with psize == 0 succeeds (CreatePipe uses a default buffer size) and a small write/read round-trips.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>

TC_CASE(zero_size)
{
    int f[2] = { -1, -1 }; char b[4] = { 0 };
    CHECK_EQ_I(_pipe(f, 0, _O_BINARY), 0);
    if (f[0] < 0) return;
    CHECK_EQ_I(_write(f[1], "ok", 2), 2);
    CHECK_EQ_I(_read(f[0], b, 2), 2);
    CHECK_MEM(b, "ok", 2);
    _close(f[0]); _close(f[1]);
}

int main(void)
{
    tc_begin();
    TC_RUN(zero_size);
    return tc_end();
}
