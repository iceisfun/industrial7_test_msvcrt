/* TC
 * id:    io_commit_pipe_probe
 * area:  lowio
 * funcs: _commit _pipe _close
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _commit, _pipe
 * note:  conf probe: _commit on the write end of an anonymous _pipe. FlushFileBuffers on a pipe write handle with no pending data is expected to succeed, so _commit returns 0. Nothing is written (a flush with unread data would block).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(commit_pipe_write_end)
{
    int fds[2];
    CHECK_EQ_I(_pipe(fds, 4096, _O_BINARY), 0);
    CHECK_EQ_I(_commit(fds[1]), 0);
    _close(fds[0]); _close(fds[1]);
}
int main(void) { tc_begin(); TC_RUN(commit_pipe_write_end); return tc_end(); }
