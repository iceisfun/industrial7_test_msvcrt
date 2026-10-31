/* TC
 * id:    tm_wstrdate_wstrtime_basic
 * area:  time
 * funcs:
 * procs: _wstrdate _wstrtime _wstrdate_s _wstrtime_s
 * crt:   >=90 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strdate, _wstrdate_s, _wstrtime_s
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(wchar_t *, _wstrdate, (wchar_t *));
TC_DECL_PROC(wchar_t *, _wstrtime, (wchar_t *));
TC_DECL_PROC(int, _wstrdate_s, (wchar_t *, size_t));
TC_DECL_PROC(int, _wstrtime_s, (wchar_t *, size_t));
static int dig(wchar_t c) { return c >= L'0' && c <= L'9'; }
static size_t wlen(const wchar_t *s) { size_t n = 0; while (s[n]) n++; return n; }
static void shape(const wchar_t *b, wchar_t sep)
{
    CHECK_EQ_U(wlen(b), 8);
    CHECK(dig(b[0]) && dig(b[1]) && dig(b[3]) && dig(b[4]) && dig(b[6]) && dig(b[7]));
    CHECK(b[2] == sep && b[5] == sep);
}
TC_CASE(date_s) { wchar_t b[9]; if (!tc_p__wstrdate_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__wstrdate_s(b, 9), 0); shape(b, L'/'); }
TC_CASE(time_s) { wchar_t b[9]; if (!tc_p__wstrtime_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__wstrtime_s(b, 9), 0); shape(b, L':'); }
TC_CASE(date_plain)
{
    wchar_t b[9]; wchar_t *r;
    if (!tc_p__wstrdate) TC_SKIP("missing");
    r = tc_p__wstrdate(b); CHECK_EQ_P(r, b); shape(b, L'/');
}
TC_CASE(time_plain)
{
    wchar_t b[9]; wchar_t *r;
    if (!tc_p__wstrtime) TC_SKIP("missing");
    r = tc_p__wstrtime(b); CHECK_EQ_P(r, b); shape(b, L':');
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wstrdate); TC_LOAD_PROC(_wstrtime); TC_LOAD_PROC(_wstrdate_s); TC_LOAD_PROC(_wstrtime_s);
    TC_RUN(date_s); TC_RUN(time_s); TC_RUN(date_plain); TC_RUN(time_plain);
    return tc_end();
}
