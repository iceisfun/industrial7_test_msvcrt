/* TC
 * id:    ucr_neg_stdio_vfprintf_readonly_errno_probe
 * area:  ucrt_only
 * funcs: fopen fclose remove fputs ferror
 * procs: __stdio_common_vfprintf
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fprintf, errno constants
 * note:  PROBE: ferror() is set and errno is EBADF after writing to a read-only stream, and the invalid-parameter handler runs at most once
 *        (exact count 0 or 1 undetermined; on failure the message prints the count).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vfprintf_readonly_errno_probe_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(errno_ebadf)
{
    FILE *f; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("orig", f); fclose(f);
    f = fopen(TMP, "rb"); CHECK(f != NULL); if (!f) { remove(TMP); return; }
    errno = 0; tc_iph_count = 0;
    r = callf(f, "X%d", 1);
    CHECK(r < 0);
    CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(f) != 0);
    CHECK(tc_iph_count == 0 || tc_iph_count == 1);
    fclose(f); remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    TC_RUN(errno_ebadf);
    return tc_end();
}
