/* TC
 * id:    io_neg_open_osfhandle_bogus_probe
 * area:  lowio
 * funcs: _open_osfhandle _close
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open_osfhandle (returns -1 on error; validation of the handle is not documented)
 * note:  PROBE: _open_osfhandle on a bogus (never-valid) handle value 0x7FFFFFF0. Assumed (from memory of the CRT
 *        sources, not documented): the CRT validates the handle with GetFileType and returns -1 with errno EBADF for
 *        FILE_TYPE_UNKNOWN. If real Windows returns a descriptor instead, this probe documents that. Only the return
 *        value is probed (errno asserted in io_neg_open_osfhandle_bad).
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>

TC_CASE(bogus_handle)
{
    volatile intptr_t h = (intptr_t)0x7FFFFFF0;
    int fd;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    fd = _open_osfhandle(h, 0);
    CHECK_EQ_I(fd, -1);
    if (fd >= 0) (void)_close(fd);
}

int main(void) { tc_begin(); TC_RUN(bogus_handle); return tc_end(); }
