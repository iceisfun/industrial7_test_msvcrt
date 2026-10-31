/* TC
 * id:    io_neg_fstat64_null_buf_probe
 * area:  lowio
 * funcs: _fstat64 _open _close _set_invalid_parameter_handler
 * procs: _fstat64i32
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fstat, _fstat64 (buffer validation not explicit)
 * note:  PROBES: _fstat64(validfd, NULL) and _fstat64i32(validfd, NULL) return -1, set errno EINVAL and call the
 *        invalid-parameter handler exactly once.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_DECL_PROC(int, _fstat64i32, (int, struct _stat64i32 *));

TC_CASE(null_buf)
{
    struct _stat64 *volatile p = NULL; int fd = _open("NUL", _O_RDWR);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_fstat64(fd, p), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _close(fd);
}
TC_CASE(null_buf_i32)
{
    struct _stat64i32 *volatile p = NULL; int fd;
    if (!tc_p__fstat64i32) TC_SKIP("_fstat64i32 not exported");
    fd = _open("NUL", _O_RDWR);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__fstat64i32(fd, p), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _close(fd);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_fstat64i32);
    TC_RUN(null_buf); TC_RUN(null_buf_i32);
    return tc_end();
}
