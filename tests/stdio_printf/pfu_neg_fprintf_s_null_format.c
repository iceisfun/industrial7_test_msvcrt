/* TC
 * id:    pfu_neg_fprintf_s_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fwprintf_s vfprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, fwprintf_s, vfprintf_s (format NULL -> invalid parameter handler, return -1, errno EINVAL)
 * note:  temp file tc_pfu_neg_fprintf_s_null_format_*.tmp, removed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define FN "tc_pfu_neg_fprintf_s_null_format_" TC_CRT_DLL ".tmp"
static FILE *fp;

static int vfa(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vfprintf_s(f, fmt, ap); va_end(ap); return r; }
static long fsize(void) { fflush(fp); fseek(fp, 0, SEEK_END); return ftell(fp); }

TC_CASE(fprintf_s_null_fmt) { volatile const char *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fprintf_s(fp, (const char *)f); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fwprintf_s_null_fmt) { volatile const wchar_t *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fwprintf_s(fp, (const wchar_t *)f); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vfprintf_s_null_fmt) { volatile const char *f = NULL; int r; tc_iph_count = 0; errno = 0; r = vfa(fp, (const char *)f, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(file_still_empty) { CHECK_EQ_I(fsize(), 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    fp = fopen(FN, "wb+"); if (!fp) { tc_write("TC FAIL " TC_TEST_ID "::__fopen\n"); return 2; }
    TC_RUN(fprintf_s_null_fmt); TC_RUN(fwprintf_s_null_fmt); TC_RUN(vfprintf_s_null_fmt); TC_RUN(file_still_empty);
    fclose(fp); DeleteFileA(FN);
    return tc_end();
}
