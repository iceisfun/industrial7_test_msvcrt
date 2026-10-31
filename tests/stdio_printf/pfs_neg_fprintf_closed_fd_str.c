/* TC
 * id:    pfs_neg_fprintf_closed_fd_str
 * area:  stdio_printf
 * funcs: fprintf fwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fflush, _close (EBADF), fprintf
 * note:  PROBE: the FILE's fd is closed behind its back with _close. fprintf/fwprintf %s must not crash; the buffered
 *        write may succeed but the later fflush must fail with EOF and errno EBADF. fclose result is ignored.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <io.h>
#include <errno.h>

#define TMP "tc_pfs_closedfd.tmp"

TC_CASE(narrow)
{
    FILE *f; volatile const char *s = "x"; int fd;
    tc_iph_install();
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    fd = _fileno(f); CHECK(fd >= 0);
    _close(fd);
    (void)fprintf(f, "%s", (const char *)s);
    errno = 0;
    CHECK_EQ_I(fflush(f), EOF);
    CHECK_EQ_I(errno, EBADF);
    (void)fclose(f);
    remove(TMP);
}
TC_CASE(wide)
{
    FILE *f; volatile const wchar_t *s = L"x"; int fd;
    tc_iph_install();
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    fd = _fileno(f); CHECK(fd >= 0);
    _close(fd);
    (void)fwprintf(f, L"%s", (const wchar_t *)s);
    errno = 0;
    CHECK_EQ_I(fflush(f), EOF);
    CHECK_EQ_I(errno, EBADF);
    (void)fclose(f);
    remove(TMP);
}
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
