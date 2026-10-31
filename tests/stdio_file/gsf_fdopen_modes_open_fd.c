/* TC
 * id:    gsf_fdopen_modes_open_fd
 * area:  stdio_file
 * funcs: _fdopen _open _close _write _read fread fwrite fclose fflush _fileno
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fdopen (associates a stream with an open file descriptor; fclose closes the descriptor)
 * note:  Complements fio_fdopen_basic. Descriptors come from _open with explicit binary mode.
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
#define F "tc_gsf_fdopen_modes_open_fd_" SFX ".tmp"

TC_CASE(r_reads)
{
    char b[8]; int fd; FILE *f; size_t n;
    mk(F, "hello", 5);
    fd = _open(F, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    f = _fdopen(fd, "rb");
    CHECK(f != NULL);
    if (f) {
        CHECK_EQ_I(_fileno(f), fd);
        n = fread(b, 1, 8, f); CHECK_EQ_U(n, 5); CHECK_MEM(b, "hello", 5);
        fclose(f);
    } else if (fd >= 0) _close(fd);
    DeleteFileA(F);
}
TC_CASE(w_writes)
{
    char b[8]; int fd, fd2; FILE *f;
    mk(F, "", 0);
    fd = _open(F, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    f = _fdopen(fd, "wb");
    CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fwrite("abc", 1, 3, f), 3);
        CHECK_EQ_I(fflush(f), 0);
        fd2 = _open(F, _O_RDONLY | _O_BINARY);
        CHECK(fd2 >= 0);
        if (fd2 >= 0) { CHECK_EQ_I(_read(fd2, b, 8), 3); CHECK_MEM(b, "abc", 3); _close(fd2); }
        CHECK_EQ_I(fclose(f), 0);
    } else if (fd >= 0) _close(fd);
    DeleteFileA(F);
}
TC_CASE(rplus_rw)
{
    char b[8], o[16]; int fd; FILE *f;
    mk(F, "abcdef", 6);
    fd = _open(F, _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    f = _fdopen(fd, "r+b");
    CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fread(b, 1, 2, f), 2); CHECK_MEM(b, "ab", 2);
        CHECK_EQ_I(fseek(f, 4, SEEK_SET), 0);
        CHECK_EQ_U(fwrite("XY", 1, 2, f), 2);
        CHECK_EQ_I(fclose(f), 0);
    } else if (fd >= 0) _close(fd);
    CHECK_EQ_I(rd(F, o, sizeof o), 6); CHECK_STR(o, "abcdXY");
    DeleteFileA(F);
}
TC_CASE(append)
{
    char o[16]; int fd; FILE *f;
    mk(F, "abc", 3);
    fd = _open(F, _O_WRONLY | _O_APPEND | _O_BINARY);
    CHECK(fd >= 0);
    f = _fdopen(fd, "ab");
    CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fwrite("def", 1, 3, f), 3);
        CHECK_EQ_I(fclose(f), 0);
    } else if (fd >= 0) _close(fd);
    CHECK_EQ_I(rd(F, o, sizeof o), 6); CHECK_STR(o, "abcdef");
    DeleteFileA(F);
}
TC_CASE(fclose_closes_fd)
{
    int fd, r; FILE *f;
    mk(F, "abc", 3);
    fd = _open(F, _O_RDWR | _O_BINARY);
    f = _fdopen(fd, "r+b");
    CHECK(f != NULL);
    if (f) {
        CHECK_EQ_I(fclose(f), 0);
        errno = 0; r = _write(fd, "z", 1);
        CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    } else if (fd >= 0) _close(fd);
    DeleteFileA(F);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(r_reads); TC_RUN(w_writes); TC_RUN(rplus_rw); TC_RUN(append); TC_RUN(fclose_closes_fd);
    return tc_end();
}
