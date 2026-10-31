/* TC
 * id:    pfo_neg_vswprintf_s_toosmall
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnwprintf_s
 * procs: __stdio_common_vswprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swprintf_s, vswprintf_s, _snwprintf_s ("buffer[0] set to 0 on failure", _TRUNCATE truncation returns -1 with STRUNCATE and no handler)
 * note:  errno for too-small buffer is ERANGE per the sprintf_s error-condition table. errno after _TRUNCATE truncation is not asserted
 *        (undocumented for the printf family; STRUNCATE is only documented for the str*_s copy functions).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vswprintf_s_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
static vswprintf_s_t p_vswprintf_s;
static int vs(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vswprintf_s(0, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}
static int vns(wchar_t *b, size_t n, size_t cnt, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vsnwprintf_s(0, b, n, cnt, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(too_small)
{
    wchar_t b[4] = { L'x', L'x', L'x', L'x' }; int r;
    tc_iph_count = 0; errno = 0;
    r = vs(b, 4, L"%s", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vs(b, 4, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(size_zero)
{
    wchar_t b[4] = { L'x', L'x', L'x', L'x' }; volatile size_t n = 0; int r;
    tc_iph_count = 0; errno = 0;
    r = vs(b, n, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(truncate_no_handler)
{
    wchar_t b[4] = { L'x', L'x', L'x', L'x' }; int r;
    tc_iph_count = 0; errno = 0;
    r = vns(b, 4, (size_t)-1, L"%s", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_WSTR(b, L"abc");
}
int main(void)
{
    tc_begin();
    p_vswprintf_s = (vswprintf_s_t)tc_crt_proc("__stdio_common_vswprintf_s");
    if (!p_vswprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    if (!tc_iph_install()) return 2;
    TC_RUN(too_small); TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(truncate_no_handler);
    return tc_end();
}
