/* TC
 * id:    prc_neg_fd_noinherit
 * area:  process
 * funcs: _spawnl _open _write _close
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open: _O_NOINHERIT prevents the file descriptor from being inherited by a child process
 * note:  Child "nfd_<n>" must see _write fail with -1/EBADF; child "yfd_<n>" (control, plain fd) must succeed.
 *        Children install the invalid-parameter handler so a bad fd returns rather than aborts.
 */
#include "tc.h"
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_prc_neg_fd_noinherit_" TC_S(TC_CRT_VER) ".tmp"

static int del_retry(const char *fn)
{
    int i; for (i = 0; i < 40; i++) { if (DeleteFileA(fn) || GetFileAttributesA(fn) == INVALID_FILE_ATTRIBUTES) return 1; Sleep(50); }
    return 0;
}

static int num_after(const char *s)
{
    int n = 0; while (*s && (*s < '0' || *s > '9')) s++;
    while (*s >= '0' && *s <= '9') n = n * 10 + (*s++ - '0');
    return n;
}
static long file_size(const char *fn)
{
    HANDLE h = CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s; if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h); return (long)s;
}
/* returns child exit status, or -1000 if open failed; *size = resulting file size */
static int run(int extra, const char *prefix, long *size)
{
    char exe[MAX_PATH], arg[64]; int i, fd; intptr_t r;
    fd = _open(FN, _O_BINARY | _O_CREAT | _O_TRUNC | _O_WRONLY | extra, _S_IREAD | _S_IWRITE);
    if (fd < 0) return -1000;
    for (i = 0; prefix[i]; i++) arg[i] = prefix[i];
    { int v = fd, k = 0, t[8]; do { t[k++] = v % 10; v /= 10; } while (v); while (k) arg[i++] = (char)('0' + t[--k]); }
    arg[i] = 0;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    r = _spawnl(_P_WAIT, exe, exe, arg, NULL);
    _close(fd);
    *size = file_size(FN);
    del_retry(FN);
    return (int)r;
}

TC_CASE(noinherit_child_gets_ebadf)
{
    long sz = -5; int r = run(_O_NOINHERIT, "--tc-child=nfd_", &sz);
    CHECK_EQ_I(r, 0);     /* child saw -1/EBADF */
    CHECK_EQ_I(sz, 0);    /* nothing written */
}

TC_CASE(control_inherited_works)
{
    long sz = -5; int r = run(0, "--tc-child=yfd_", &sz);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(sz, 1);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int fd = num_after(c); int w;
        tc_iph_install();
        errno = 0;
        w = _write(fd, "X", 1);
        if (c[0] == 'n') return (w == -1 && errno == EBADF) ? 0 : 1;
        return w == 1 ? 0 : 1;
    }
    TC_RUN(noinherit_child_gets_ebadf);
    TC_RUN(control_inherited_works);
    return tc_end();
}
