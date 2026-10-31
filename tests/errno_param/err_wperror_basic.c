/* TC
 * id:    err_wperror_basic
 * area:  errno_param
 * funcs: _wperror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs perror, _wperror
 * note:  PROBE: _wperror writes a narrow ASCII "abc: No such file or directory\n" to stderr (encoding of stderr output); NULL prefix prints message only (handler installed in case a CRT validates NULL).
 */
#include "tc.h"

#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

static int  saved_fd = -1;
static char tmpname[64];
static void cap_begin(const char *name)
{
    int fd;
    strcpy(tmpname, name);
    fd = _open(tmpname, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    saved_fd = _dup(2);
    if (fd >= 0 && saved_fd >= 0) _dup2(fd, 2);
    if (fd >= 0) _close(fd);
}
static int cap_end(char *out, int cap)
{
    int fd, n = 0;
    if (saved_fd >= 0) { _dup2(saved_fd, 2); _close(saved_fd); saved_fd = -1; }
    fd = _open(tmpname, _O_RDONLY | _O_BINARY);
    if (fd >= 0) { n = _read(fd, out, cap - 1); _close(fd); }
    if (n < 0) n = 0;
    out[n] = 0;
    remove(tmpname);
    return n;
}

TC_CASE(prefix)
{
    char b[256]; volatile const wchar_t *p = L"abc";
    cap_begin("tc_err_wperror_basic_1.tmp");
    errno = ENOENT; _wperror((const wchar_t *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "abc: No such file or directory\n");
}
TC_CASE(null_prefix)
{
    char b[256]; volatile const wchar_t *p = NULL;
    cap_begin("tc_err_wperror_basic_2.tmp");
    errno = ENOENT; _wperror((const wchar_t *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "No such file or directory\n");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(prefix); TC_RUN(null_prefix); return tc_end(); }
