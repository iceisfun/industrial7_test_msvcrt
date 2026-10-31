/* TC
 * id:    pfo_neg_probe_vfprintf_wide_oriented_stream_narrow
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf __stdio_common_vfwprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen (ccs=UNICODE, BOM written on create); Unicode stream I/O (byte-oriented functions on Unicode streams not specified)
 * note:  PROBES: byte-oriented fprintf on a "w, ccs=UNICODE" stream is rejected: negative return, EINVAL, handler runs once.
 *        Separate case: a wide call alone on such a stream yields a file of exactly BOM(2) + 2*chars bytes (no narrow call involved).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define FN "tc_pfo_neg_vfprintf_wide_oriented_" TC_CRT_DLL ".tmp"
static int ca(FILE *f, const char *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfprintf(0, f, fm, NULL, ap); va_end(ap); return r; }
static int cw(FILE *f, const wchar_t *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfwprintf(0, f, fm, NULL, ap); va_end(ap); return r; }
static long fsize(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s; if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h); return (long)s;
}
TC_CASE(narrow_rejected)
{
    FILE *f = fopen(FN, "w, ccs=UNICODE"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = ca(f, "abc");
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(FN);
}
TC_CASE(wide_control_and_size)
{
    FILE *f = fopen(FN, "w, ccs=UNICODE"); int r2; CHECK(f != NULL); if (!f) return;
    r2 = cw(f, L"hi");
    CHECK_EQ_I(r2, 2);
    fclose(f);
    CHECK_EQ_I(fsize(FN), 2 + 2 * 2);
    remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_rejected); TC_RUN(wide_control_and_size);
    return tc_end();
}
