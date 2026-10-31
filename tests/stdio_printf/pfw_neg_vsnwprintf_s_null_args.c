/* TC
 * id:    pfw_neg_vsnwprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snwprintf_s, _vsnwprintf_s: invalid buffer/sizeOfBuffer/format -> invalid parameter handler, -1, EINVAL
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
static wchar_t * volatile nbuf;
static const wchar_t * volatile nfmt;
static const wchar_t * volatile xfmt = L"x";

TC_CASE(null_buffer) { int r; tc_iph_count = 0; errno = 0; r = vsnws(nbuf, 8, 8, xfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { wchar_t b[8] = { L'Q', 0 }; int r; tc_iph_count = 0; errno = 0; r = vsnws(b, 8, 8, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format_truncate) { wchar_t b[8] = { L'Q', 0 }; int r; tc_iph_count = 0; errno = 0; r = vsnws(b, 8, _TRUNCATE, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(control) { wchar_t b[8]; CHECK_EQ_I(vsnws(b, 8, 8, L"%d", 123), 3); CHECK_WSTR(b, L"123"); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vsnwprintf_s = (vsnws_t)tc_crt_proc("_vsnwprintf_s");
    CHECK(p_vsnwprintf_s != NULL);
#endif
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(null_format_truncate); TC_RUN(control);
    return tc_end();
}
