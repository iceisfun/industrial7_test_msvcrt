/* TC
 * id:    pfu_neg_printf_s_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf_s wprintf_s vprintf_s vwprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_s, wprintf_s, vprintf_s, vwprintf_s (format NULL -> invalid parameter handler, -1, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vp(const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vprintf_s(fmt, ap); va_end(ap); return r; }
static int vw(const wchar_t *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vwprintf_s(fmt, ap); va_end(ap); return r; }

TC_CASE(printf_s_null)   { volatile const char *f = NULL; int r; tc_iph_count = 0; errno = 0; r = printf_s((const char *)f); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wprintf_s_null)  { volatile const wchar_t *f = NULL; int r; tc_iph_count = 0; errno = 0; r = wprintf_s((const wchar_t *)f); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vprintf_s_null)  { volatile const char *f = NULL; int r; tc_iph_count = 0; errno = 0; r = vp((const char *)f, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vwprintf_s_null) { volatile const wchar_t *f = NULL; int r; tc_iph_count = 0; errno = 0; r = vw((const wchar_t *)f, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(printf_s_null); TC_RUN(wprintf_s_null); TC_RUN(vprintf_s_null); TC_RUN(vwprintf_s_null);
    return tc_end();
}
