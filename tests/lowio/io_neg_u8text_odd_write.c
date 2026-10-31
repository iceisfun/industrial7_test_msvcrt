/* TC
 * id:    io_neg_u8text_odd_write
 * area:  lowio
 * funcs: _open _write _close _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _write (Unicode modes require an even byte count)
 * note:  Probes that _O_U8TEXT (wide input, UTF-8 output) also rejects an odd byte count with EINVAL; docs only spell this out for UTF-16.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, (DWORD)cap, &r, NULL); CloseHandle(h); return (int)r;
}
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define N "tc_io_neg_u8text_odd_write_" TC_CRT_DLL ".tmp"

TC_CASE(write_odd)
{
    int fd, r; char buf[4] = { 'a', 0, 'b', 0 };
    CHECK(tc_iph_install());
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_U8TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    tc_iph_count = 0; errno = 0;
    r = _write(fd, buf, 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _close(fd); rm(N);
}

int main(void)
{
    tc_begin();
    TC_RUN(write_odd);
    return tc_end();
}
