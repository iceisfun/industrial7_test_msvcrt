/* TC
 * id:    pfu_neg_fprintf_s_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fwprintf_s vfprintf_s vfwprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, fwprintf_s, vfprintf_s, vfwprintf_s (stream NULL -> invalid parameter handler, return -1, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vfa(FILE *f, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vfprintf_s(f, fmt, ap); va_end(ap); return r; }
static int vfw(FILE *f, const wchar_t *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vfwprintf_s(f, fmt, ap); va_end(ap); return r; }

TC_CASE(fprintf_s_null)  { volatile FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fprintf_s((FILE *)f, "x"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fwprintf_s_null) { volatile FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = fwprintf_s((FILE *)f, L"x"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vfprintf_s_null) { volatile FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = vfa((FILE *)f, "%d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vfwprintf_s_null){ volatile FILE *f = NULL; int r; tc_iph_count = 0; errno = 0; r = vfw((FILE *)f, L"%d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(fprintf_s_null); TC_RUN(fwprintf_s_null); TC_RUN(vfprintf_s_null); TC_RUN(vfwprintf_s_null);
    return tc_end();
}
