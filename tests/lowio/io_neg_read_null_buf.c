/* TC
 * id:    io_neg_read_null_buf
 * area:  lowio
 * funcs: _read _open _close _write _lseek
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read; Parameter Validation (buffer NULL -> EINVAL, invalid-parameter handler)
 */
#include "tc.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_read_null_buf_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(null_buffer)
{
    void * volatile nb = NULL;
    int fd, r;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    tc_iph_count = 0; errno = 0;
    r = _read(fd, nb, 5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 0);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_buffer); return tc_end(); }
