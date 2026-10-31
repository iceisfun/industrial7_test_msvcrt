/* TC
 * id:    gsm_strset_strnset_basic
 * area:  string
 * funcs: _strset _strnset
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strset, _strnset (_strnset sets at most count characters, stops at the terminator; returns the altered string)
 */
#include "tc.h"
#include <string.h>

static void guard(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = 'G'; }

TC_CASE(strset_all)
{
    char b[12]; char *volatile r; guard(b, 12);
    b[0] = 'a'; b[1] = 'b'; b[2] = 'c'; b[3] = 0;
    r = _strset(b, 'x'); CHECK_STR(b, "xxx"); CHECK_EQ_P(r, b); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'G');
}
TC_CASE(strset_empty)
{
    char b[4] = ""; char *volatile r = _strset(b, 'x'); CHECK_EQ_P(r, b); CHECK_EQ_I(b[0], 0);
}
TC_CASE(nset_partial)
{
    char b[12]; char *volatile r; guard(b, 12);
    b[0] = 'a'; b[1] = 'b'; b[2] = 'c'; b[3] = 'd'; b[4] = 0;
    r = _strnset(b, 'z', 2); CHECK_STR(b, "zzcd"); CHECK_EQ_P(r, b);
}
TC_CASE(nset_equal)
{
    char b[12]; guard(b, 12);
    b[0] = 'a'; b[1] = 'b'; b[2] = 'c'; b[3] = 0;
    _strnset(b, 'q', 3); CHECK_STR(b, "qqq"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'G');
}
TC_CASE(nset_larger)
{
    char b[12]; guard(b, 12);
    b[0] = 'a'; b[1] = 'b'; b[2] = 0;
    _strnset(b, 'q', 10); CHECK_STR(b, "qq"); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], 'G'); CHECK_EQ_I(b[11], 'G');
}
TC_CASE(nset_zero)
{
    char b[8] = "abc"; char *volatile r = _strnset(b, 'z', 0); CHECK_STR(b, "abc"); CHECK_EQ_P(r, b);
}
TC_CASE(high_fill)
{
    char b[8] = "abc";
    _strset(b, 0xE9);
    CHECK_EQ_I((unsigned char)b[0], 0xE9); CHECK_EQ_I((unsigned char)b[1], 0xE9); CHECK_EQ_I((unsigned char)b[2], 0xE9); CHECK_EQ_I(b[3], 0);
    _strnset(b, 0x80, 1);
    CHECK_EQ_I((unsigned char)b[0], 0x80); CHECK_EQ_I((unsigned char)b[1], 0xE9);
}
int main(void)
{
    tc_begin(); TC_RUN(strset_all); TC_RUN(strset_empty); TC_RUN(nset_partial); TC_RUN(nset_equal);
    TC_RUN(nset_larger); TC_RUN(nset_zero); TC_RUN(high_fill); return tc_end();
}
