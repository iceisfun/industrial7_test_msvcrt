/* TC
 * id:    pfo_neg_vswprintf_s_toosmall
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnwprintf_s
 * procs: __stdio_common_vswprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs swprintf_s, vswprintf_s, _snwprintf_s ("buffer[0] set to 0 on failure", _TRUNCATE truncation returns -1 with STRUNCATE and no handler)
 * note:  FIXED test bug (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "too_small r == -1 got=1", zzp_probe2 "vswprintf_s small %s(wide arg): r=1 w=[a]" vs
 *        "vswprintf_s small %ls: r=-1 errno=34 iph=1 b0=0" and "vsnwprintf_s TRUNC %ls: r=-1 iph=0 w=[abc]"): with options 0 the wide printf family uses ISO
 *        specifiers, so %s consumes a NARROW string; the test passed a wide string to %s (read as "a"). Now %ls (explicitly wide) is used; the
 *        original expectations (ERANGE, handler once, buf[0]=0, _TRUNCATE gives -1 and "abc" without handler) all hold unchanged.
 *        Errno after _TRUNCATE truncation is not asserted (undocumented). Stable.
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
    r = vs(b, 4, L"%ls", L"abcdefgh");
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
    r = vns(b, 4, (size_t)-1, L"%ls", L"abcdefgh");
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
