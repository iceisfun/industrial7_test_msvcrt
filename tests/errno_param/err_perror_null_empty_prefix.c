/* TC
 * id:    err_perror_null_empty_prefix
 * area:  errno_param
 * funcs: perror
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs perror: if string is NULL or points to an empty string only the message is printed
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

TC_CASE(null_prefix)
{
    char b[256]; volatile const char *p = NULL;
    cap_begin("tc_err_perror_null_empty_prefix_1.tmp");
    errno = ENOENT; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "No such file or directory\n");
}
TC_CASE(empty_prefix)
{
    char b[256]; volatile const char *p = "";
    cap_begin("tc_err_perror_null_empty_prefix_2.tmp");
    errno = ENOENT; perror((const char *)p);
    cap_end(b, sizeof b);
    CHECK_STR(b, "No such file or directory\n");
}
int main(void) { tc_begin(); TC_RUN(null_prefix); TC_RUN(empty_prefix); return tc_end(); }
