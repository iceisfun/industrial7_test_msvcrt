/* TC
 * id:    gdr_neg_fstat32_null_buf_probe
 * area:  dir
 * kind:  negative
 * funcs: _fstat32 _fstat32i64 _open _close
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fstat (buffer NULL treated as invalid parameter)
 * note:  PROBE: _fstat32/_fstat32i64(validfd, NULL) -> -1, EINVAL, handler invoked. The 64-bit variant is covered by lowio io_neg_fstat64_null_buf_probe.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static struct _stat32 *volatile v_n32 = NULL;
static struct _stat32i64 *volatile v_n64 = NULL;

TC_CASE(null_buf)
{
    int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_fstat32(fd, v_n32), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_fstat32i64(fd, v_n64), -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _close(fd);
}
int main(void) { tc_begin(); TC_RUN(null_buf); return tc_end(); }
