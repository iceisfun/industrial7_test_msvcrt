/* TC
 * id:    pfw_vscwprintf_length
 * area:  stdio_printf
 * funcs: _vscwprintf
 * procs: _vsnwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _vscprintf, _vscwprintf
 * note:  size, allocate len+1, format roundtrip; _vsnwprintf used only as helper in the roundtrip
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>

static int vsc(const wchar_t *fmt, ...)
{ va_list ap; int n; va_start(ap, fmt); n = _vscwprintf(fmt, ap); va_end(ap); return n; }

TC_CASE(literal)  { CHECK_EQ_I(vsc(L"abcdef"), 6); CHECK_EQ_I(vsc(L""), 0); }
TC_CASE(int_arg)  { CHECK_EQ_I(vsc(L"%d", -12345), 6); CHECK_EQ_I(vsc(L"%05d", 42), 5); }
TC_CASE(wide_str_arg) { CHECK_EQ_I(vsc(L"<%ls>", L"wide"), 6); CHECK_EQ_I(vsc(L"<%hs>", "nar"), 5); }
static wchar_t *alloc_fmt(int *plen, const wchar_t *fmt, ...)
{
    va_list ap; int n, m; wchar_t *buf;
    va_start(ap, fmt); n = _vscwprintf(fmt, ap); va_end(ap);
    *plen = n;
    if (n < 0) return NULL;
    buf = (wchar_t *)HeapAlloc(GetProcessHeap(), 0, ((size_t)n + 1) * sizeof(wchar_t));
    if (!buf) return NULL;
    va_start(ap, fmt); m = _vsnwprintf(buf, (size_t)n + 1, fmt, ap); va_end(ap);
    if (m != n) *plen = -2;
    return buf;
}
TC_CASE(roundtrip)
{
    int n; wchar_t *buf = alloc_fmt(&n, L"%ls=%d", L"key", 99);
    CHECK_EQ_I(n, 6);
    CHECK(buf != NULL);
    if (buf) { CHECK_WSTR(buf, L"key=99"); HeapFree(GetProcessHeap(), 0, buf); }
}

int main(void) { tc_begin(); TC_RUN(literal); TC_RUN(int_arg); TC_RUN(wide_str_arg); TC_RUN(roundtrip); return tc_end(); }
