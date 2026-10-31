/* TC
 * id:    pff_neg_swprintf_s_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: swprintf_s vswprintf_s __stdio_common_vswprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l, swprintf_s: too-small buffer -> invalid parameter handler, -1, buffer[0] = 0
 * note:  ucrtbase has swprintf_s only inline (wchar.h) over __stdio_common_vswprintf_s, so that export is called there.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *vs_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vs_t p_vs;
static int call_vs(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vs ? p_vs(0, b, n, f, NULL, ap) : -2; }
#define LOAD_VS() ((p_vs = (vs_t)tc_crt_proc("__stdio_common_vswprintf_s")) != NULL)
#else
typedef int (__cdecl *vs_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vs_t p_vs;
static int call_vs(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vs ? p_vs(b, n, f, ap) : -2; }
#define LOAD_VS() ((p_vs = (vs_t)tc_crt_proc("vswprintf_s")) != NULL)
#endif
static int swprintf_s_x(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = call_vs(b, n, f, ap); va_end(ap); return r; }
#define swprintf_s swprintf_s_x

TC_CASE(too_small)
{
    wchar_t wb[16]; volatile double v = 1.5; volatile size_t n = 4; int r, i;
    for (i = 0; i < 16; i++) wb[i] = 0x5A5A;
    tc_iph_count = 0;
    r = swprintf_s(wb, n, L"%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(wb[0], 0);
    CHECK_EQ_I(wb[4], 0x5A5A);
}
TC_CASE(no_room_for_nul)
{
    wchar_t wb[16]; volatile double v = 1.5; volatile size_t n = 8; int r;
    wb[0] = 0x5A5A;
    tc_iph_count = 0;
    r = swprintf_s(wb, n, L"%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(wb[0], 0);
}
TC_CASE(size9_ok)
{
    wchar_t wb[16]; volatile double v = 1.5; volatile size_t n = 9; int r;
    tc_iph_count = 0;
    r = swprintf_s(wb, n, L"%f", v);
    CHECK_EQ_I(r, 8);
    CHECK_WSTR(wb, L"1.500000");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!LOAD_VS()) { tc_write("TC FAIL " TC_TEST_ID "::__vswprintf_s not exported\n"); return 2; }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(no_room_for_nul); TC_RUN(size9_ok); return tc_end();
}
