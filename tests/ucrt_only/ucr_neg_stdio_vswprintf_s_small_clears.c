/* TC
 * id:    ucr_neg_stdio_vswprintf_s_small_clears
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf_s __stdio_common_vswprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf_s (buffer too small: buffer[0]=0, ERANGE, handler), _snwprintf (-1 on overflow, no write past count), C11 7.29.2.7 snprintf-like return via options bit 2
 * note:  The std-snprintf case uses the UCRT options bit _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR (2).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vws_t p_vws, p_vwss;
static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vws(o, b, n, fmt, NULL, ap); va_end(ap); return r; }
static int callws(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vwss(0, b, n, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(s_overflow)
{
    wchar_t b[8]; int r, i;
    if (!p_vwss) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) b[i] = L'S';
    errno = 0; tc_iph_count = 0;
    r = callws(b, 3, L"hello");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[3], L'S'); CHECK_EQ_I(b[7], L'S');
}
TC_CASE(legacy_no_overrun)
{
    wchar_t b[8]; int r, i;
    if (!p_vws) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) b[i] = L'S';
    tc_iph_count = 0;
    r = callw(0, b, 3, L"hello");
    CHECK_EQ_I(r, -1);
    for (i = 3; i < 8; i++) CHECK_EQ_I(b[i], L'S');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(std_snprintf)
{
    wchar_t b[8]; int r, i;
    if (!p_vws) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) b[i] = L'S';
    tc_iph_count = 0;
    r = callw(2, b, 3, L"hello");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"he");
    for (i = 3; i < 8; i++) CHECK_EQ_I(b[i], L'S');
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vws = (vws_t)tc_crt_proc("__stdio_common_vswprintf");
    p_vwss = (vws_t)tc_crt_proc("__stdio_common_vswprintf_s");
    TC_RUN(s_overflow); TC_RUN(legacy_no_overrun); TC_RUN(std_snprintf);
    return tc_end();
}
