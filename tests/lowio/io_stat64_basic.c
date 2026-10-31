/* TC
 * id:    io_stat64_basic
 * area:  lowio
 * funcs: _stat64 _open _write _close
 * procs: _stat64i32
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _stat, _wstat (st_size, st_mode, st_atime/st_mtime/st_ctime)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define FN "tc_io_stat64_basic.tmp"
#define DN "tc_io_stat64_basic_dir.tmp"
TC_DECL_PROC(int, _stat64i32, (const char *, struct _stat64i32 *));

TC_CASE(file)
{
    struct _stat64 st; int fd;
    rm(FN);
    fd = _open(FN, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "hello world", 11), 11);
    _close(fd);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_stat64(FN, &st), 0);
    CHECK_EQ_I(st.st_size, 11);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFREG);
    CHECK(st.st_atime > 0);
    CHECK(st.st_mtime > 0);
    CHECK(st.st_ctime > 0);
    rm(FN);
}
TC_CASE(directory)
{
    struct _stat64 st;
    RemoveDirectoryA(DN);
    CHECK(CreateDirectoryA(DN, NULL));
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_stat64(DN, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFDIR);
    CHECK(st.st_mtime > 0);
    RemoveDirectoryA(DN);
}
TC_CASE(i32_size_equal)
{
    struct _stat64 s64; struct _stat64i32 s32; int fd;
    if (!tc_p__stat64i32) TC_SKIP("_stat64i32 not exported");
    rm(FN);
    fd = _open(FN, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcdefghij", 10), 10);
    _close(fd);
    memset(&s32, 0, sizeof s32);
    CHECK_EQ_I(_stat64(FN, &s64), 0);
    CHECK_EQ_I(tc_p__stat64i32(FN, &s32), 0);
    CHECK_EQ_I(s32.st_size, 10);
    CHECK_EQ_I(s32.st_size, s64.st_size);
    CHECK_EQ_I(s32.st_mode, s64.st_mode);
    CHECK_EQ_I(s32.st_mtime, s64.st_mtime);
    rm(FN);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_stat64i32);
    TC_RUN(file); TC_RUN(directory); TC_RUN(i32_size_equal);
    return tc_end();
}
