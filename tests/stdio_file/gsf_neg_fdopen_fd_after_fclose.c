/* TC
 * id:    gsf_neg_fdopen_fd_after_fclose
 * area:  stdio_file
 * funcs: _fdopen _open _write _close fclose
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _fdopen, fclose (closes the underlying descriptor); _close/_write on an invalid descriptor -> EBADF
 * note:  Handler installed (CRT>=80 invokes it for bad descriptors); its count is not asserted. For _fdopen of a closed
 *        descriptor NULL is asserted everywhere but errno == EBADF only from msvcr80 on.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    if (l) WriteFile(h, d, l, &w, NULL);
    CloseHandle(h);
}
static int rd(const char *n, char *b, int cap)  /* bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static int vsc(const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = vscanf(f, ap); va_end(ap);
    return r;
}
#define F "tc_gsf_neg_fdopen_fd_after_fclose_" SFX ".tmp"

static int mkclosed(void)  /* returns an fd number that was valid and has been closed via fclose of its _fdopen stream */
{
    int fd; FILE *f;
    mk(F, "abc", 3);
    fd = _open(F, _O_RDWR | _O_BINARY);
    if (fd < 0) return -1;
    f = _fdopen(fd, "r+b");
    if (!f) { _close(fd); return -1; }
    fclose(f);
    return fd;
}
TC_CASE(write_after_fclose)
{
    int fd = mkclosed(), r;
    CHECK(fd >= 0);
    errno = 0; r = _write(fd, "z", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    DeleteFileA(F);
}
TC_CASE(close_after_fclose)
{
    int fd = mkclosed(), r;
    CHECK(fd >= 0);
    errno = 0; r = _close(fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    DeleteFileA(F);
}
TC_CASE(fdopen_of_closed_fd)
{
    int fd = mkclosed(); FILE *f;
    CHECK(fd >= 0);
    errno = 0; f = _fdopen(fd, "r");
    CHECK(f == NULL);
#if TC_CRT_VER >= 80
    CHECK_EQ_I(errno, EBADF);
#endif
    if (f) fclose(f);
    DeleteFileA(F);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(write_after_fclose); TC_RUN(close_after_fclose); TC_RUN(fdopen_of_closed_fd);
    return tc_end();
}
