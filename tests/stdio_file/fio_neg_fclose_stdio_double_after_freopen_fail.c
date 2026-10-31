/* TC
 * id:    fio_neg_fclose_stdio_double_after_freopen_fail
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen fopen _fileno _close _get_osfhandle
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.5.4p3 (freopen closes the file first); MS docs freopen
 * note:  Probes that after freopen fails (missing file, mode "r") the original stream's fd is closed (EBADF from _close /
 *        _get_osfhandle) and that the original file handle was not leaked (DeleteFile of the original succeeds).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <io.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define NAME "tc_fio_frfail_" TC_S1(TC_CRT_VER) ".tmp"
#define MISSING "tc_fio_frfail_missing_" TC_S1(TC_CRT_VER) ".tmp"

static void put(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, len, &w, NULL); CloseHandle(h); }
}
TC_CASE(fd_closed)
{
    FILE *f, *g; int fd, r;
    put(NAME, "abc", 3); DeleteFileA(MISSING);
    f = fopen(NAME, "r"); CHECK(f != NULL); if (!f) return;
    fd = _fileno(f);
    g = freopen(MISSING, "r", f);
    CHECK(g == NULL);
    errno = 0; r = _close(fd);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    errno = 0;
    CHECK_EQ_I(_get_osfhandle(fd), -1);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(no_handle_leak)
{
    FILE *f, *g;
    put(NAME, "abc", 3); DeleteFileA(MISSING);
    f = fopen(NAME, "r"); CHECK(f != NULL); if (!f) return;
    g = freopen(MISSING, "r", f);
    CHECK(g == NULL);
    CHECK(DeleteFileA(NAME) != 0);
    CHECK(GetFileAttributesA(NAME) == INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(fd_closed); TC_RUN(no_handle_leak);
    DeleteFileA(NAME);
    return tc_end();
}
