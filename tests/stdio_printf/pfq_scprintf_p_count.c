/* TC
 * id:    pfq_scprintf_p_count
 * area:  stdio_printf
 * funcs: _scprintf_p _vscprintf_p _scwprintf_p _vscwprintf_p _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scprintf_p, _scprintf_p_l, _scwprintf_p (number of characters, not counting the terminating null)
 * note:  ucrtbase: inline over __stdio_common_vsprintf_p(NULL, 0).
 */
#include "tc.h"
#include "pfq_shim.h"

static int vsc(const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pq_vscprintf_p(fmt, ap); va_end(ap); return r; }
static int vscw(const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = pq_vscwprintf_p(fmt, ap); va_end(ap); return r; }

TC_CASE(basic)
{
    CHECK_EQ_I(pq_scprintf_p("%2$s%1$s", "ab", "cd"), 4);
    CHECK_EQ_I(pq_scprintf_p("%1$d", -123), 4);
    CHECK_EQ_I(pq_scprintf_p("%1$s%1$s%1$s", "abc"), 9);
}
TC_CASE(matches_sprintf)
{
    char b[64]; int n = pq_sprintf_p(b, sizeof b, "%2$s/%1$5d", 42, "dir");
    CHECK_EQ_I(n, 9);
    CHECK_EQ_I(vsc("%2$s/%1$5d", 42, "dir"), n);
    CHECK_EQ_I(pq_scprintf_p("%2$s/%1$5d", 42, "dir"), n);
}
TC_CASE(wide)
{
    CHECK_EQ_I(pq_scwprintf_p(L"%2$ls%1$ls", L"ab", L"cde"), 5);
    CHECK_EQ_I(vscw(L"%2$ls-%1$d", 77, L"xy"), 5);
}
TC_CASE(empty)
{
    const char * volatile e = ""; const wchar_t * volatile w = L"";
    CHECK_EQ_I(pq_scprintf_p(e), 0);
    CHECK_EQ_I(pq_scwprintf_p(w), 0);
}
TC_CASE(long_output)
{
    int n = pq_scprintf_p("%1$.*2$s", "x", 1);
    CHECK_EQ_I(n, 1);
    n = pq_scprintf_p("%1$1000s", "x");
    CHECK_EQ_I(n, 1000);
    n = pq_scprintf_p("%1$*2$d", 5, 1000);
    CHECK_EQ_I(n, 1000);
}
TC_CASE(no_side_effects)
{
    int a = pq_scprintf_p("%2$s%1$d", 12345, "abc");
    int b = pq_scprintf_p("%2$s%1$d", 12345, "abc");
    CHECK_EQ_I(a, 8);
    CHECK_EQ_I(b, 8);
}
int main(void)
{
    tc_begin();
    TC_RUN(basic); TC_RUN(matches_sprintf); TC_RUN(wide); TC_RUN(empty); TC_RUN(long_output); TC_RUN(no_side_effects);
    return tc_end();
}
