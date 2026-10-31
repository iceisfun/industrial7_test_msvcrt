/* TC
 * id:    ucr_neg_stdio_vfwprintf_readonly_ebadf_probe
 * area:  ucrt_only
 * funcs: fopen fclose remove fputs
 * procs: __stdio_common_vfwprintf
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fwprintf; errno values
 * note:  PROBES: errno after vfwprintf to a stream opened "r" is EBADF (stream not open for writing). Return < 0 is asserted too.
 *        temp file tc_ucr_neg_stdio_vfwprintf_readonly_ebadf_probe_*.tmp
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vfw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vfw_t p_vfw;
#define TMP "tc_ucr_neg_stdio_vfwprintf_readonly_ebadf_probe_a.tmp"
static int callfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(errno_ebadf)
{
    FILE *f; int r;
    if (!p_vfw) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("orig", f); fclose(f);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { remove(TMP); return; }
    errno = 0;
    r = callfw(f, L"NEW%d", 1);
    CHECK(r < 0); CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vfw = (vfw_t)tc_crt_proc("__stdio_common_vfwprintf");
    TC_RUN(errno_ebadf);
    return tc_end();
}
