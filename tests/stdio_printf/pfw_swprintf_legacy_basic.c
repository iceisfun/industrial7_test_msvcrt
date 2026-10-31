/* TC
 * id:    pfw_swprintf_legacy_basic
 * area:  stdio_printf
 * funcs: _swprintf
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs swprintf, _swprintf (non-count form: _swprintf(buf, fmt, ...))
 * note:  msvcr80 and msvcrt excluded (import lib incomplete; msvcrt.dll _swprintf export unverified). ucrtbase: _swprintf is a header inline over __stdio_common_vswprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

TC_CASE(basic_ints)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _swprintf(b, L"%d,%x,%05d", -5, 255, 42);
    CHECK_WSTR(b, L"-5,ff,00042"); CHECK_EQ_I(n, 11);
}
TC_CASE(wide_str)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _swprintf(b, L"<%ls|%hs>", L"wide", "narrow");
    CHECK_WSTR(b, L"<wide|narrow>"); CHECK_EQ_I(n, 13);
}
TC_CASE(return_len) { wchar_t b[32]; CHECK_EQ_I(_swprintf(b, L"%ls", L"abcdefghij"), 10); }
TC_CASE(writes_NUL)
{
    wchar_t b[8]; int n; fill(b, 8);
    n = _swprintf(b, L"ab");
    CHECK_EQ_I(n, 2); CHECK(b[2] == 0); CHECK(b[3] == L'Z');
}

int main(void) { tc_begin(); TC_RUN(basic_ints); TC_RUN(wide_str); TC_RUN(return_len); TC_RUN(writes_NUL); return tc_end(); }
