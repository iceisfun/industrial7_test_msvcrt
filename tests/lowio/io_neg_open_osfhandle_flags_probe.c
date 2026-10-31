/* TC
 * id:    io_neg_open_osfhandle_flags_probe
 * area:  lowio
 * funcs: _open_osfhandle _close
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open_osfhandle
 * note:  PROBE: _open_osfhandle(valid handle, 0x40000000) with an undefined flag bit. Assumed: flags are not validated
 *        and a descriptor is returned.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>

static char N[] = "tc_io_neg_open_osfhandle_flags_probe.tmp";

TC_CASE(bad_flags)
{
    HANDLE h = CreateFileA(N, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    volatile int flags = 0x40000000;
    int fd;
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h == INVALID_HANDLE_VALUE) return;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    fd = _open_osfhandle((intptr_t)h, flags);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd); else CloseHandle(h);
    DeleteFileA(N);
}

int main(void) { tc_begin(); TC_RUN(bad_flags); return tc_end(); }
