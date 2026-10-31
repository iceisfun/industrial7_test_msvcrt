/* TC
 * id:    scn_neg_snwscanf_guard_page
 * area:  scanf
 * kind:  negative
 * funcs: _snwscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf, _snwscanf (count = maximum number of characters to read; the buffer need not be NUL terminated)
 * note:  The wide source ends exactly at a PAGE_NOACCESS page: reading even one wchar_t beyond count faults (= test fail).
 *        On ucrtbase _snwscanf is an inline wrapper over __stdio_common_vswscanf.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#ifdef _UCRT
#include <stdarg.h>
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
#endif

static volatile size_t cnt;
static unsigned char *base;

/* copy n wide chars so that they end exactly at the guard page */
static wchar_t *place(const wchar_t *src, size_t n)
{
    wchar_t *p = (wchar_t *)(base + 4096 - n * sizeof(wchar_t)); size_t i;
    for (i = 0; i < n; i++) p[i] = src[i];
    return p;
}

TC_CASE(string_then_int)
{
    wchar_t w[16]; int v = 0, r; wchar_t *s = place(L"ab 12", 5);
    w[0] = 0; cnt = 5; r = _snwscanf(s, cnt, L"%ls %d", w, &v);
    CHECK_EQ_I(r, 2); CHECK_WSTR(w, L"ab"); CHECK_EQ_I(v, 12);
}
TC_CASE(int_cut_by_count)
{
    int v = 0, r; wchar_t *s = place(L"12345", 5);
    cnt = 3; r = _snwscanf(s, cnt, L"%d", &v);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123);
}
TC_CASE(string_cut_by_count)
{
    wchar_t w[16]; int r; wchar_t *s = place(L"abcdef", 6);
    w[0] = 0; cnt = 4; r = _snwscanf(s, cnt, L"%ls", w);
    CHECK_EQ_I(r, 1); CHECK_WSTR(w, L"abcd");
}
TC_CASE(exhausted_before_second)
{
    int a = 0, b = 77, r; wchar_t *s = place(L"9 ", 2);
    cnt = 2; r = _snwscanf(s, cnt, L"%d %d", &a, &b);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 9); CHECK_EQ_I(b, 77);
}

int main(void)
{
    DWORD old;
    tc_begin();
    base = (unsigned char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!base || !VirtualProtect(base + 4096, 4096, PAGE_NOACCESS, &old)) { tc_write("TC FAIL " TC_TEST_ID "::__setup guard page\n"); return 2; }
    TC_RUN(string_then_int); TC_RUN(int_cut_by_count); TC_RUN(string_cut_by_count); TC_RUN(exhausted_before_second);
    return tc_end();
}
