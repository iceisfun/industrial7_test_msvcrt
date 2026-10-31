/* TC
 * id:    io_neg_open_dir
 * area:  lowio
 * funcs: _open _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: EACCES (tried to open a read-only file for writing / sharing violation)
 * note:  Probes that _open on a directory fails with EACCES (CreateFile on a directory without BACKUP_SEMANTICS gives ERROR_ACCESS_DENIED, mapped to EACCES). Not explicitly documented.
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

TC_CASE(dir_rdonly) { int fd; errno = 0; fd = _open(".", _O_RDONLY); CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EACCES); if (fd >= 0) _close(fd); }
TC_CASE(dir_rdwr)   { int fd; errno = 0; fd = _open(".", _O_RDWR);   CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EACCES); if (fd >= 0) _close(fd); }
int main(void) { tc_begin(); TC_RUN(dir_rdonly); TC_RUN(dir_rdwr); return tc_end(); }
