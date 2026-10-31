/* TC
 * id:    io_neg_open_invalid_oflag
 * area:  lowio
 * funcs: _open _close _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: _O_WTEXT/_O_U16TEXT/_O_U8TEXT/_O_TEXT/_O_BINARY
 * note:  Probes which combinations of text-mode oflags the UCRT rejects with EINVAL + invalid parameter handler. Reasoning only (ucrt open validates the text-mode bits); each combination is its own case. _O_BINARY|_O_TEXT was removed: the CRT is believed to let one flag win silently rather than reject it.
 */
#include "tc.h"
#include <stdio.h>
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

#define N "tc_io_neg_open_invalid_oflag_" TC_CRT_DLL ".tmp"
static void bad(int oflag)
{
    int fd;
    CHECK(tc_iph_install());
    rm(N); tc_iph_count = 0; errno = 0;
    fd = _open(N, oflag | _O_CREAT | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (fd >= 0) _close(fd);
    rm(N);
}
TC_CASE(wtext_u16)    { bad(_O_WTEXT | _O_U16TEXT); }
TC_CASE(u8_u16)       { bad(_O_U8TEXT | _O_U16TEXT); }
int main(void) { tc_begin(); TC_RUN(wtext_u16); TC_RUN(u8_u16); return tc_end(); }
