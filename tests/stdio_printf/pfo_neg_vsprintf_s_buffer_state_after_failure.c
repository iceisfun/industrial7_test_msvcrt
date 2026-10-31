/* TC
 * id:    pfo_neg_vsprintf_s_buffer_state_after_failure
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s __stdio_common_vswprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: if the buffer is too small, buffer[0] is set to NUL and the invalid parameter handler runs; NULL format -> EINVAL
 * note:  Bytes beyond the stated buffer size must never be written.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>

int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vswprintf_s(unsigned __int64, wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile size_t sz4 = 4;
static const char * volatile nfmt;
static const wchar_t * volatile nwfmt;

TC_CASE(narrow)
{
    char b[16]; int r, i; memset(b, 'Z', sizeof b);
    tc_iph_count = 0; r = cs(b, sz4, "abcdefgh");
    CHECK(r < 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(wide)
{
    wchar_t b[16]; int r, i; for (i = 0; i < 16; i++) b[i] = L'Z';
    tc_iph_count = 0; r = cw(b, sz4, L"abcdefgh");
    CHECK(r < 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], L'Z');
}
TC_CASE(null_format)
{
    char b[16]; wchar_t w[16]; int r;
    tc_iph_count = 0; errno = 0; r = cs(b, 16, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; r = cw(w, 16, nwfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow); TC_RUN(wide); TC_RUN(null_format);
    return tc_end();
}
