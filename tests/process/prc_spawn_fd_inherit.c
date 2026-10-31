/* TC
 * id:    prc_spawn_fd_inherit
 * area:  process
 * funcs: _spawnl _open _write _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn, _open: open file descriptors are inherited by the spawned process
 * note:  Child (same exe, name "wfd_<n>") writes one byte to inherited descriptor n and exits 0 on success.
 *        The child installs the invalid-parameter handler so an fd failure returns instead of aborting.
 */
#include "tc.h"
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_prc_spawn_fd_inherit_" TC_S(TC_CRT_VER) ".tmp"

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
static char first_byte(const char *fn)
{
    HANDLE h = CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    char b = 0; DWORD n = 0; if (h == INVALID_HANDLE_VALUE) return 0;
    ReadFile(h, &b, 1, &n, NULL); CloseHandle(h); return n == 1 ? b : 0;
}

TC_CASE(child_writes_inherited_fd)
{
    char exe[MAX_PATH], arg[64]; const char *p = "--tc-child=wfd_"; int i, fd; volatile int one = 1; intptr_t r; char dig[8];
    fd = _open(FN, _O_BINARY | _O_CREAT | _O_TRUNC | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    for (i = 0; p[i]; i++) arg[i] = p[i];
    { int v = fd, k = 0, t[8]; do { t[k++] = v % 10; v /= 10; } while (v); while (k) arg[i++] = (char)('0' + t[--k]); }
    arg[i] = 0; (void)dig; (void)one;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    r = _spawnl(_P_WAIT, exe, exe, arg, NULL);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(_close(fd), 0);
    CHECK_EQ_I(file_size(FN), 1);
    CHECK_EQ_I(first_byte(FN), 'X');
    del_retry(FN);
    CHECK_EQ_I(file_size(FN), -1);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int fd = num_after(c);
        tc_iph_install();
        return _write(fd, "X", 1) == 1 ? 0 : 1;
    }
    TC_RUN(child_writes_inherited_fd);
    return tc_end();
}
