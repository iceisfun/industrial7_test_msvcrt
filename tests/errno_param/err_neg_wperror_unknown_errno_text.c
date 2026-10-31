/* TC
 * id:    err_neg_wperror_unknown_errno_text
 * area:  errno_param
 * kind:  negative
 * funcs: _wperror
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs perror, _wperror
 * note:  PROBE: with errno 9999 (outside the table) _wperror still writes one line ending in "\n" to stderr and does not crash;
 *        the exact text is not asserted, only that it is non-empty (prefix "pfx: " first when a prefix is given).
 *        Either narrow or UTF-16LE byte encoding of the captured stderr output is accepted.
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

/* _wperror's byte encoding on fd 2 is not documented: accept narrow text or UTF-16LE text. */
static int ends_nl(const char *b, int n)
{ return (n >= 1 && b[n - 1] == '\n') || (n >= 2 && b[n - 2] == '\n' && b[n - 1] == 0); }
static int starts_pfx(const char *b, int n)
{ return (n >= 5 && memcmp(b, "pfx: ", 5) == 0) || (n >= 10 && memcmp(b, "p\0f\0x\0:\0 \0", 10) == 0); }

TC_CASE(prefix_unknown)
{
    char b[256]; volatile const wchar_t *p = L"pfx"; int n;
    cap_begin("tc_err_neg_wperror_1.tmp");
    errno = 9999; _wperror((const wchar_t *)p);
    n = cap_end(b, sizeof b);
    CHECK(n > 6);
    CHECK(starts_pfx(b, n));
    CHECK(ends_nl(b, n));
}
TC_CASE(null_prefix_unknown)
{
    char b[256]; volatile const wchar_t *p = NULL; int n;
    cap_begin("tc_err_neg_wperror_2.tmp");
    errno = 9999; _wperror((const wchar_t *)p);
    n = cap_end(b, sizeof b);
    CHECK(n > 1);
    CHECK(ends_nl(b, n));
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(prefix_unknown); TC_RUN(null_prefix_unknown); errno = 0; return tc_end(); }
