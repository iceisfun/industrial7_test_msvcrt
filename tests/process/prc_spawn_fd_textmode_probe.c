/* TC
 * id:    prc_spawn_fd_textmode_probe
 * area:  process
 * funcs: _spawnl _open _write _close
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _spawn, _open, _write (text mode translates LF to CR-LF)
 * note:  Probes that the per-descriptor text/binary flag travels to the child (via STARTUPINFO.lpReserved2):
 *        child writes "\n" to the inherited fd; the file is 1 byte for a binary parent fd and 2 bytes for a text fd.
 */
#include "tc.h"
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_prc_spawn_fd_textmode_probe_" TC_S(TC_CRT_VER) ".tmp"

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
static long run(int flags)
{
    char exe[MAX_PATH], arg[64]; const char *p = "--tc-child=nl_"; int i, fd; long sz; intptr_t r;
    fd = _open(FN, flags | _O_CREAT | _O_TRUNC | _O_WRONLY, _S_IREAD | _S_IWRITE);
    if (fd < 0) return -100;
    for (i = 0; p[i]; i++) arg[i] = p[i];
    { int v = fd, k = 0, t[8]; do { t[k++] = v % 10; v /= 10; } while (v); while (k) arg[i++] = (char)('0' + t[--k]); }
    arg[i] = 0;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    r = _spawnl(_P_WAIT, exe, exe, arg, NULL);
    _close(fd);
    sz = r == 0 ? file_size(FN) : -200;
    del_retry(FN);
    return sz;
}

TC_CASE(binary_fd) { CHECK_EQ_I(run(_O_BINARY), 1); }
TC_CASE(text_fd)   { CHECK_EQ_I(run(_O_TEXT), 2); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int fd = num_after(c);
        tc_iph_install();
        return _write(fd, "\n", 1) != -1 ? 0 : 1;
    }
    TC_RUN(binary_fd);
    TC_RUN(text_fd);
    return tc_end();
}
