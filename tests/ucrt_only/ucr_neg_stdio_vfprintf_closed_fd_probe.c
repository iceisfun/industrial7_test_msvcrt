/* TC
 * id:    ucr_neg_stdio_vfprintf_closed_fd_probe
 * area:  ucrt_only
 * funcs: fopen fclose fflush remove _close _fileno
 * procs: __stdio_common_vfprintf
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _close, fflush, fprintf
 * note:  PROBES: after _close(_fileno(f)) behind the CRT's back, the FILE still buffers writes, so vfprintf itself may
 *        succeed; but vfprintf or the following fflush must report failure, and errno (if set) must be EBADF.
 *        The handler is installed (returns), so a bad-fd parameter check cannot terminate the process.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vfprintf_closed_fd_probe_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(closed_fd)
{
    FILE *f; int r, fr;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_close(_fileno(f)), 0);
    errno = 0;
    r = callf(f, "hello %d", 1);
    if (r >= 0) errno = 0;
    fr = fflush(f);
    CHECK(r < 0 || fr == EOF);
    if (errno != 0) CHECK_EQ_I(errno, EBADF);
    fclose(f);   /* may fail; result ignored */
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    TC_RUN(closed_fd);
    return tc_end();
}
