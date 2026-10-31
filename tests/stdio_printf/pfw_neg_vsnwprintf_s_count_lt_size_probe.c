/* TC
 * id:    pfw_neg_vsnwprintf_s_count_lt_size_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnwprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s: count smaller than the output (and smaller than sizeOfBuffer) truncates to count chars
 * note:  PROBE. buf[8], count=3, 20-char input: probes that the call returns -1 with "012" + NUL in the buffer and no handler call.
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
static const wchar_t * volatile s20 = L"0123456789abcdefghij";

TC_CASE(count3)
{
    wchar_t b[8]; int r; tc_iph_count = 0;
    r = vsnws(b, 8, 3, L"%ls", s20);
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"012"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vsnwprintf_s = (vsnws_t)tc_crt_proc("_vsnwprintf_s");
    CHECK(p_vsnwprintf_s != NULL);
#endif
    TC_RUN(count3); return tc_end();
}
