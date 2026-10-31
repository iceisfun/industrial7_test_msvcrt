/* TC
 * id:    err_neg_perror_closed_stderr
 * area:  errno_param
 * kind:  negative
 * funcs: perror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs perror
 * note:  PROBE: perror with fd 2 closed neither crashes nor terminates the process (a handler is installed so a fatal default cannot fire); errno and whether the invalid-parameter handler runs (_write on a closed fd may invoke it) are unspecified and not asserted. fd 2 is restored.
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

TC_CASE(closed)
{
    int sv = _dup(2); volatile const char *p = "x";
    CHECK(sv >= 0);
    if (sv < 0) return;
    _close(2);
    tc_iph_count = 0;
    errno = ENOENT; perror((const char *)p);
    _dup2(sv, 2); _close(sv);
    CHECK(1);  /* reaching here is the assertion; whether the CRT reports EBADF through the invalid-parameter handler is unspecified, so tc_iph_count is not asserted */
}
int main(void) { tc_begin(); if (TC_AT_LEAST(80)) tc_iph_install(); TC_RUN(closed); return tc_end(); }
