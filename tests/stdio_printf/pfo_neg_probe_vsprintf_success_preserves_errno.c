/* TC
 * id:    pfo_neg_probe_vsprintf_success_preserves_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vswprintf __stdio_common_vfprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.5p3 (errno MAY be set by a library function even on success unless documented, so this is not guaranteed by the standard)
 * note:  PROBES: successful narrow, wide and stream printf calls leave errno untouched (expected in practice, undocumented).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define TMP "tc_pfo_neg_probe_vsprintf_success_errno.tmp"
static int c0(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cw(unsigned o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cf(FILE *fp, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf(0, fp, f, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow) { char b[16]; int r; errno = 12345; r = c0(0, b, sizeof b, "%d", 7); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 12345); }
TC_CASE(wide)   { wchar_t b[16]; int r; errno = 12345; r = cw(0, b, 16, L"%d", 7); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 12345); }
TC_CASE(stream)
{
    FILE *fp = fopen(TMP, "w"); int r;
    if (!fp) TC_SKIP("cannot open temp");
    errno = 12345; r = cf(fp, "%d", 7);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 12345);
    fclose(fp); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    TC_RUN(narrow); TC_RUN(wide); TC_RUN(stream);
    return tc_end();
}
