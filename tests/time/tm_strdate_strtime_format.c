/* TC
 * id:    tm_strdate_strtime_format
 * area:  time
 * funcs: _strdate _strtime
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strdate, _strtime (MM/DD/YY and HH:MM:SS, buffer of at least 9 bytes)
 * note:  shape only; values are not compared against the clock
 */
#include "tc.h"
#include <time.h>

static int dig(char c) { return c >= '0' && c <= '9'; }
static int num2(const char *p) { return (p[0] - '0') * 10 + (p[1] - '0'); }

TC_CASE(strdate)
{
    char b[16]; char *r; int i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = _strdate(b);
    CHECK_EQ_P(r, b);
    CHECK(dig(b[0]) && dig(b[1]) && dig(b[3]) && dig(b[4]) && dig(b[6]) && dig(b[7]));
    CHECK_EQ_I(b[2], '/'); CHECK_EQ_I(b[5], '/'); CHECK_EQ_I(b[8], 0);
    CHECK(num2(b) >= 1 && num2(b) <= 12);
    CHECK(num2(b + 3) >= 1 && num2(b + 3) <= 31);
}
TC_CASE(strtime)
{
    char b[16]; char *r; int i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = _strtime(b);
    CHECK_EQ_P(r, b);
    CHECK(dig(b[0]) && dig(b[1]) && dig(b[3]) && dig(b[4]) && dig(b[6]) && dig(b[7]));
    CHECK_EQ_I(b[2], ':'); CHECK_EQ_I(b[5], ':'); CHECK_EQ_I(b[8], 0);
    CHECK(num2(b) <= 23); CHECK(num2(b + 3) <= 59); CHECK(num2(b + 6) <= 61);
}
TC_CASE(exact_nine_bytes)
{
    char b[9 + 4]; int i;
    for (i = 0; i < 13; i++) b[i] = 'Q';
    _strdate(b);
    CHECK_EQ_I(b[8], 0); CHECK_EQ_I(b[9], 'Q');
    for (i = 0; i < 13; i++) b[i] = 'Q';
    _strtime(b);
    CHECK_EQ_I(b[8], 0); CHECK_EQ_I(b[9], 'Q');
}
int main(void) { tc_begin(); TC_RUN(strdate); TC_RUN(strtime); TC_RUN(exact_nine_bytes); return tc_end(); }
