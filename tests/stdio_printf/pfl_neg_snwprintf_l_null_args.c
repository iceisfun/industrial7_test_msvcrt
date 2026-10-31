/* TC
 * id:    pfl_neg_snwprintf_l_null_args
 * area:  stdio_printf
 * funcs: _vsnwprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation"; _snprintf, _snprintf_l, _snwprintf_l, _vsnwprintf_l (NULL format / NULL buffer with count > 0: EINVAL, -1)
 * note:  Wide counterpart of pfl_neg_vsprintf_l_null. On ucrtbase the SDK inlines the wide functions over __stdio_common_vswprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

static _locale_t loc;

static int vsn(wchar_t *b, size_t n, const wchar_t *f, ...)
{
    va_list ap; int r; va_start(ap, f);
#if TC_CRT_VER >= 140
    r = __stdio_common_vswprintf(5ULL, b, n, f, loc, ap);
#else
    r = _vsnwprintf_l(b, n, f, loc, ap);
#endif
    va_end(ap); return r;
}

static int snw(wchar_t *b, size_t n, const wchar_t *f, ...)
{
    va_list ap; int r; va_start(ap, f);
#if TC_CRT_VER >= 140
    r = __stdio_common_vswprintf(5ULL, b, n, f, loc, ap);   /* what the SDK's inline _snwprintf_l does */
#else
    r = _vsnwprintf_l(b, n, f, loc, ap);
#endif
    va_end(ap); return r;
}

TC_CASE(snwprintf_l_null_fmt)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    b[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = snw(b, 8, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(snwprintf_l_null_buf)
{
    wchar_t *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = snw(b, 5, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vsnwprintf_l_null_fmt)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vsn(b, 8, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(vsnwprintf_l_null_buf)
{
    wchar_t *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vsn(b, 5, L"x", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_after_failures)
{
    wchar_t b[16]; int r;
    tc_iph_count = 0;
    r = snw(b, 16, L"%d-%ls", 42, L"ok");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"42-ok"); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(snwprintf_l_null_fmt); TC_RUN(snwprintf_l_null_buf); TC_RUN(vsnwprintf_l_null_fmt);
    TC_RUN(vsnwprintf_l_null_buf); TC_RUN(valid_after_failures);
    _free_locale(loc);
    return tc_end();
}
