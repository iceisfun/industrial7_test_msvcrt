/* TC
 * id:    pfu_neg_sprintf_s_zero_size
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s swprintf_s vsprintf_s vswprintf_s _vsnprintf_s _vsnwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_s, swprintf_s, vsprintf_s, _vsnprintf_s (sizeOfBuffer); size 0 is not explicitly documented
 * note:  PROBE: sizeOfBuffer == 0 with a non-NULL buffer and non-NULL format. Believed: invalid-parameter handler
 *        called once, return -1, buffer untouched (no room even for the NUL). The docs describe the handler only for
 *        NULL args and too-small buffers, so this is kept apart from the documented cases.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static char fmt_x[] = "x";
static char fmt_e[] = "";
static wchar_t wfmt_x[] = L"x";

static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static int cn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int cnw(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(sprintf_zero_x)
{
    char b[8] = "QQQ"; volatile size_t n = 0; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, n, fmt_x);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(sprintf_zero_empty)
{
    char b[8] = "QQQ"; volatile size_t n = 0; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, n, fmt_e);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(swprintf_zero)
{
    wchar_t w[4] = { L'Q', L'Q', L'Q', L'Q' }; volatile size_t n = 0; int r;
    tc_iph_count = 0;
    r = swprintf_s(w, n, wfmt_x);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(w[0], L'Q');
}
TC_CASE(vsprintf_zero)
{
    char b[8] = "QQ"; volatile size_t n = 0; int r;
    tc_iph_count = 0;
    r = cv(b, n, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(vswprintf_zero)
{
    wchar_t w[8] = { L'Q', L'Q' }; volatile size_t n = 0; int r;
    tc_iph_count = 0;
    r = cw(w, n, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(w[0], L'Q');
}
TC_CASE(vsnprintf_s_zero)
{
    char b[8] = "QQ"; volatile size_t n = 0, c = 4; int r;
    tc_iph_count = 0;
    r = cn(b, n, c, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Q');
}
TC_CASE(vsnwprintf_s_zero)
{
    wchar_t w[8] = { L'Q', L'Q' }; volatile size_t n = 0, c = 4; int r;
    tc_iph_count = 0;
    r = cnw(w, n, c, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(w[0], L'Q');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_zero_x); TC_RUN(sprintf_zero_empty); TC_RUN(swprintf_zero);
    TC_RUN(vsprintf_zero); TC_RUN(vswprintf_zero); TC_RUN(vsnprintf_s_zero); TC_RUN(vsnwprintf_s_zero);
    return tc_end();
}
