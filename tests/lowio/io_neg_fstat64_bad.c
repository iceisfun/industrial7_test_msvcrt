/* TC
 * id:    io_neg_fstat64_bad
 * area:  lowio
 * funcs: _fstat64 _open _close _set_invalid_parameter_handler
 * procs: _fstat64i32
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _fstat, _fstat64 (invalid fd -> EBADF; Parameter validation)
 * note:  the NULL-buffer case is in io_neg_fstat64_null_buf_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

TC_DECL_PROC(int, _fstat64i32, (int, struct _stat64i32 *));

TC_CASE(bad_fd)
{
    struct _stat64 st; volatile int fd = -1;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_fstat64(fd, &st), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(closed_fd)
{
    struct _stat64 st; int fd = _open("NUL", _O_RDWR);
    CHECK(fd >= 0);
    if (fd < 0) return;
    _close(fd);
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_fstat64(fd, &st), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(i32_variant)
{
    struct _stat64i32 st;
    if (!tc_p__fstat64i32) TC_SKIP("_fstat64i32 not exported");
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__fstat64i32(-1, &st), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_fstat64i32);
    TC_RUN(bad_fd); TC_RUN(closed_fd); TC_RUN(i32_variant);
    return tc_end();
}
