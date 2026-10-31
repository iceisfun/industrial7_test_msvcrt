/* TC
 * id:    io_stat64_trailing_slash_probe
 * area:  lowio
 * funcs: _stat64 _open _close
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented)
 * note:  PROBES: (1) _stat64("file/") on a regular file fails with -1/ENOENT; (2) _stat64("f*") (wildcard in name) fails with
 *        -1/ENOENT even though a matching file exists; (3) on ucrtbase only, _stat64("dir\\") on a directory succeeds as S_IFDIR.
 *        Each probe is its own case.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define FN "tc_io_stat64_ts_probe.tmp"
#define DN "tc_io_stat64_ts_probe_dir.tmp"

static void mkfile(void)
{
    int fd; rm(FN);
    fd = _open(FN, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
}
TC_CASE(file_slash)
{
    struct _stat64 st; mkfile(); errno = 0;
    CHECK_EQ_I(_stat64(FN "/", &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    rm(FN);
}
TC_CASE(wildcard)
{
    struct _stat64 st; mkfile(); errno = 0;
    CHECK_EQ_I(_stat64("tc_io_stat64_ts_probe.t*", &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    errno = 0;
    CHECK_EQ_I(_stat64("tc_io_stat64_ts_probe.tm?", &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    rm(FN);
}
TC_CASE(dir_backslash)
{
    struct _stat64 st;
    if (TC_BELOW(140)) TC_SKIP("only probed on ucrtbase");
    RemoveDirectoryA(DN);
    CHECK(CreateDirectoryA(DN, NULL));
    CHECK_EQ_I(_stat64(DN "\\", &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFDIR);
    RemoveDirectoryA(DN);
}
int main(void) { tc_begin(); TC_RUN(file_slash); TC_RUN(wildcard); TC_RUN(dir_backslash); return tc_end(); }
