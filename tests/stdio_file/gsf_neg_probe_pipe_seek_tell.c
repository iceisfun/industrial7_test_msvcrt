/* TC
 * id:    gsf_neg_probe_pipe_seek_tell
 * area:  stdio_file
 * funcs: _fdopen _fseek_nolock _ftelli64_nolock fclose
 * procs: _open_osfhandle
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lseek (fails on non-seekable devices such as pipes); _fdopen
 * note:  PROBE: seek/tell on a stream built over an anonymous pipe fail (believed). The exact errno
 *        (EINVAL most likely, from ERROR_INVALID_FUNCTION; ESPIPE also accepted) is asserted only in its own case.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
int __cdecl _fseek_nolock(FILE *, long, int);
__int64 __cdecl _ftelli64_nolock(FILE *);
FILE *__cdecl _fdopen(int, const char *);
TC_DECL_PROC(int, _open_osfhandle, (intptr_t, int));

static HANDLE wr;
static FILE *mkpipe(void)
{
    HANDLE r = NULL; int fd; FILE *f;
    wr = NULL;
    if (!CreatePipe(&r, &wr, NULL, 0)) return NULL;
    fd = tc_p__open_osfhandle((intptr_t)r, _O_RDONLY);
    if (fd < 0) { CloseHandle(r); CloseHandle(wr); wr = NULL; return NULL; }
    f = _fdopen(fd, "r");
    return f;
}
static void done(FILE *f) { if (f) fclose(f); if (wr) CloseHandle(wr); wr = NULL; }

TC_CASE(tell_pipe)
{
    FILE *f;
    if (!tc_p__open_osfhandle) TC_SKIP("_open_osfhandle not exported");
    f = mkpipe(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_ftelli64_nolock(f), -1);
    done(f);
}
TC_CASE(seek_pipe)
{
    FILE *f;
    if (!tc_p__open_osfhandle) TC_SKIP("_open_osfhandle not exported");
    f = mkpipe(); CHECK(f != NULL); if (!f) return;
    CHECK(_fseek_nolock(f, 0, SEEK_SET) != 0);
    done(f);
}
TC_CASE(errno_value_probe)
{
    FILE *f;
    if (!tc_p__open_osfhandle) TC_SKIP("_open_osfhandle not exported");
    f = mkpipe(); CHECK(f != NULL); if (!f) return;
    errno = 0;
    _ftelli64_nolock(f);
    CHECK(errno == EINVAL || errno == ESPIPE);
    done(f);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_open_osfhandle);
    TC_RUN(tell_pipe); TC_RUN(seek_pipe); TC_RUN(errno_value_probe);
    return tc_end();
}
