/* TC
 * id:    err_neg_open_dir_write_probe
 * area:  errno_param
 * funcs: _open
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open (errno values)
 * note:  PROBE: _open of a directory with _O_RDWR fails with errno EACCES.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>

TC_CASE(dir_rdwr)
{
    int fd; errno = 0;
    fd = _open(".", _O_RDWR);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EACCES);
    if (fd >= 0) _close(fd);
}
int main(void) { tc_begin(); TC_RUN(dir_rdwr); return tc_end(); }
