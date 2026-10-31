/* TC
 * id:    io_neg_write_null_buf
 * area:  lowio
 * funcs: _write _open _close _filelength
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _write; Parameter Validation (buffer NULL -> EINVAL, handler)
 * note:  count 0 with NULL buffer deliberately not tested here
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_write_null_buf_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(null_buffer)
{
    void * volatile nb = NULL;
    int fd, r;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcd", 4), 4);
    tc_iph_count = 0; errno = 0;
    r = _write(fd, nb, 5);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_filelength(fd), 4);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_buffer); return tc_end(); }
