/* TC
 * id:    err_perror_stderr_format
 * area:  errno_param
 * funcs: perror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs perror: prints "string: message\n" to stderr
 * note:  fd 2 is redirected to a temp file with _dup/_dup2 and restored afterwards; file is removed.
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

TC_CASE(enoent)
{
    char b[256]; volatile const char *p = "abc";
    cap_begin("tc_err_perror_stderr_format_1.tmp");
    errno = ENOENT; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "abc: No such file or directory\n");
}
TC_CASE(einval)
{
    char b[256]; volatile const char *p = "x";
    cap_begin("tc_err_perror_stderr_format_2.tmp");
    errno = EINVAL; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "x: Invalid argument\n");
}
TC_CASE(append)
{
    char b[256]; volatile const char *p = "a";
    cap_begin("tc_err_perror_stderr_format_3.tmp");
    errno = ENOENT; perror((const char *)p);
    errno = EINVAL; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "a: No such file or directory\na: Invalid argument\n");
}
int main(void) { tc_begin(); TC_RUN(enoent); TC_RUN(einval); TC_RUN(append); return tc_end(); }
