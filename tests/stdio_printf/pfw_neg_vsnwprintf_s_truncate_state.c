/* TC
 * id:    pfw_neg_vsnwprintf_s_truncate_state
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _vsnwprintf_s: _TRUNCATE truncates and returns -1 without invoking the handler; overflow otherwise -> handler, buf[0]=0, ERANGE
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
extern int __cdecl __stdio_common_vsnwprintf_s(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, _locale_t, va_list);
static int sh_vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(4, b, sz, cnt, f, NULL, ap); }
#else
typedef int (__cdecl *vsnws_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static vsnws_t p_vsnwprintf_s;
static int sh_vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_vsnwprintf_s ? p_vsnwprintf_s(b, sz, cnt, f, ap) : -99; }
#endif
static int vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = sh_vsnws(b, sz, cnt, f, ap); va_end(ap); return r; }
static const wchar_t * volatile lfmt = L"%ls";
static const wchar_t * volatile s20 = L"0123456789abcdefghij";
static const wchar_t * volatile s7 = L"abcdefg";

TC_CASE(truncate_flag)
{
    wchar_t b[8]; int r; tc_iph_count = 0;
    r = vsnws(b, 8, _TRUNCATE, lfmt, s20);
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"0123456"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(overflow_without_truncate)
{
    wchar_t b[8] = { L'Q', L'Q', L'Q', 0 }; int r; tc_iph_count = 0; errno = 0;
    r = vsnws(b, 8, 8, lfmt, s20);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit_truncate)
{
    wchar_t b[8]; int r; tc_iph_count = 0; errno = 0;
    r = vsnws(b, 8, _TRUNCATE, lfmt, s7);
    CHECK_EQ_I(r, 7); CHECK_WSTR(b, L"abcdefg"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(one_too_long_truncate)
{
    wchar_t b[8]; int r; tc_iph_count = 0;
    r = vsnws(b, 8, _TRUNCATE, L"%ls!", s7);
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abcdefg"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vsnwprintf_s = (vsnws_t)tc_crt_proc("_vsnwprintf_s");
    CHECK(p_vsnwprintf_s != NULL);
#endif
    TC_RUN(truncate_flag); TC_RUN(overflow_without_truncate); TC_RUN(exact_fit_truncate); TC_RUN(one_too_long_truncate);
    return tc_end();
}
