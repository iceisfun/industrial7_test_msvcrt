/* TC
 * id:    err_perror_errno_zero_unknown
 * area:  errno_param
 * funcs: perror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs perror / strerror
 * note:  PROBE: errno 0 yields "s: No error\n" and an out-of-range errno (9999) yields text containing "Unknown error".
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

TC_CASE(zero)
{
    char b[256]; volatile const char *p = "s";
    cap_begin("tc_err_perror_errno_zero_unknown_1.tmp");
    errno = 0; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "s: No error\n");
}
TC_CASE(unknown)
{
    char b[256]; volatile const char *p = "s";
    cap_begin("tc_err_perror_errno_zero_unknown_2.tmp");
    errno = 9999; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK(strstr(b, "Unknown error") != NULL);
}
int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(unknown); return tc_end(); }
