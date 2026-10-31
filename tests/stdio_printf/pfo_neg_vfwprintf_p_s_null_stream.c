/* TC
 * id:    pfo_neg_vfwprintf_p_s_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfwprintf_p __stdio_common_vfwprintf_s fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, _fprintf_p, fwprintf_s: NULL stream or format -> invalid parameter handler, returns -1, errno EINVAL
 * note:  Direct calls to the UCRT workers with options 0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
#define FN "tc_pfo_neg_vfwprintf_p_s_null_stream_" TC_CRT_DLL ".tmp"

static int cs(FILE *f, const wchar_t *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfwprintf_s(0, f, fm, NULL, ap); va_end(ap); return r; }
static int cp(FILE *f, const wchar_t *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfwprintf_p(0, f, fm, NULL, ap); va_end(ap); return r; }
static FILE * volatile nfile;
static const wchar_t * volatile nfmt;

TC_CASE(s_null_stream) { int r; tc_iph_count = 0; errno = 0; r = cs(nfile, L"%d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(p_null_stream) { int r; tc_iph_count = 0; errno = 0; r = cp(nfile, L"%1$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(s_null_format)
{
    FILE *f = fopen(FN, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = cs(f, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(FN);
}
TC_CASE(p_null_format)
{
    FILE *f = fopen(FN, "w"); int r; long pos; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = cp(f, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fflush(f); pos = ftell(f); CHECK_EQ_I(pos, 0);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(s_null_stream); TC_RUN(p_null_stream); TC_RUN(s_null_format); TC_RUN(p_null_format);
    return tc_end();
}
