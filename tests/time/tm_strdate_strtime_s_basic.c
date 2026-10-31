/* TC
 * id:    tm_strdate_strtime_s_basic
 * area:  time
 * funcs:
 * procs: _strdate_s _strtime_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strdate_s, _strtime_s
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(int, _strdate_s, (char *, size_t));
TC_DECL_PROC(int, _strtime_s, (char *, size_t));
static int dig(char c) { return c >= '0' && c <= '9'; }
static int num2(const char *p) { return (p[0] - '0') * 10 + (p[1] - '0'); }
static size_t len(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(exported)
{
    if (TC_BELOW(80)) TC_SKIP("msvcrt.dll export of the _s variants not asserted");
    CHECK(tc_p__strdate_s != NULL); CHECK(tc_p__strtime_s != NULL);
}
TC_CASE(date_9)
{
    char b[9]; int r;
    if (!tc_p__strdate_s) TC_SKIP("missing");
    r = tc_p__strdate_s(b, 9);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(len(b), 8);
    CHECK(dig(b[0]) && dig(b[1]) && dig(b[3]) && dig(b[4]) && dig(b[6]) && dig(b[7]));
    CHECK_EQ_I(b[2], '/'); CHECK_EQ_I(b[5], '/');
    CHECK(num2(b) >= 1 && num2(b) <= 12); CHECK(num2(b + 3) >= 1 && num2(b + 3) <= 31);
}
TC_CASE(time_9)
{
    char b[9]; int r;
    if (!tc_p__strtime_s) TC_SKIP("missing");
    r = tc_p__strtime_s(b, 9);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(len(b), 8);
    CHECK(dig(b[0]) && dig(b[1]) && dig(b[3]) && dig(b[4]) && dig(b[6]) && dig(b[7]));
    CHECK_EQ_I(b[2], ':'); CHECK_EQ_I(b[5], ':');
    CHECK(num2(b) <= 23 && num2(b + 3) <= 59);
}
TC_CASE(larger_buffer)
{
    char b[64]; int i;
    if (!tc_p__strdate_s || !tc_p__strtime_s) TC_SKIP("missing");
    for (i = 0; i < 64; i++) b[i] = 'Z';
    CHECK_EQ_I(tc_p__strdate_s(b, 64), 0); CHECK_EQ_U(len(b), 8);
    for (i = 0; i < 64; i++) b[i] = 'Z';
    CHECK_EQ_I(tc_p__strtime_s(b, 64), 0); CHECK_EQ_U(len(b), 8);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strdate_s); TC_LOAD_PROC(_strtime_s);
    TC_RUN(exported); TC_RUN(date_9); TC_RUN(time_9); TC_RUN(larger_buffer);
    return tc_end();
}
