/* TC
 * id:    pfo_neg_probe_vfprintf_closed_handle_write_error
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf __stdio_common_vfwprintf fopen fclose setvbuf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf (returns negative on output error); _write (EBADF for invalid handle)
 * note:  PROBES: with an unbuffered stream whose OS handle was closed behind the CRT's back, the write fails, the call returns a negative value, ferror(f)
 *        is set, errno is EBADF and the invalid parameter handler does not run.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <io.h>
#include <errno.h>

#define FA "tc_pfo_neg_probe_vfprintf_closed_a_" TC_CRT_DLL ".tmp"
#define FW "tc_pfo_neg_probe_vfprintf_closed_w_" TC_CRT_DLL ".tmp"
static int ca(FILE *f, const char *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfprintf(0, f, fm, NULL, ap); va_end(ap); return r; }
static int cw(FILE *f, const wchar_t *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfwprintf(0, f, fm, NULL, ap); va_end(ap); return r; }
static FILE *open_closed(const char *n)
{
    FILE *f = fopen(n, "w");
    if (!f) return NULL;
    setvbuf(f, NULL, _IONBF, 0);
    CloseHandle((HANDLE)_get_osfhandle(_fileno(f)));
    return f;
}
TC_CASE(narrow)
{
    FILE *f = open_closed(FA); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = ca(f, "abc");
    CHECK(r < 0); CHECK(ferror(f) != 0); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); remove(FA);
}
TC_CASE(wide)
{
    FILE *f = open_closed(FW); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = cw(f, L"abc");
    CHECK(r < 0); CHECK(ferror(f) != 0); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); remove(FW);
}
int main(void)
{
    tc_begin(); remove(FA); remove(FW);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow); TC_RUN(wide);
    return tc_end();
}
