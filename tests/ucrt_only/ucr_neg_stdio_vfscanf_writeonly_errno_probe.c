/* TC
 * id:    ucr_neg_stdio_vfscanf_writeonly_errno_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: fopen fclose remove
 * procs: __stdio_common_vfscanf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fscanf / _filbuf (reading a stream not opened for reading)
 * note:  PROBES that reading from a "w" stream sets errno to EBADF (return EOF and target untouched are in
 *        ucr_neg_stdio_vfscanf_bad_stream). temp file tc_ucr_neg_stdio_vfscanf_writeonly_errno_probe_a.tmp
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *vfscanf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vfscanf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vfscanf_writeonly_errno_probe_a.tmp"

static int xf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(errno_ebadf)
{
    FILE *f = fopen(TMP, "w"); int a = 77, r;
    CHECK(f != NULL); if (!f) return;
    errno = 0;
    r = xf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(TMP);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vfscanf_t)tc_crt_proc("__stdio_common_vfscanf");
    if (!p_vf) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(errno_ebadf);
    return tc_end();
}
