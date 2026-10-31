/* TC
 * id:    gsm_strlwr_strupr_basic
 * area:  string
 * funcs: _strlwr _strupr
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strlwr, _strupr (in-place, returns pointer to the altered string); C locale tolower/toupper affect only A-Z/a-z
 */
#include "tc.h"
#include <string.h>

TC_CASE(mixed)
{
    char b[32] = "Hello, World_[`{@ 123 xYz";
    char *volatile r;
    r = _strlwr(b); CHECK_STR(b, "hello, world_[`{@ 123 xyz"); CHECK_EQ_P(r, b);
    r = _strupr(b); CHECK_STR(b, "HELLO, WORLD_[`{@ 123 XYZ"); CHECK_EQ_P(r, b);
}
TC_CASE(empty)
{
    char b[4] = ""; char *volatile r;
    r = _strlwr(b); CHECK_EQ_P(r, b); CHECK_EQ_I(b[0], 0);
    r = _strupr(b); CHECK_EQ_P(r, b); CHECK_EQ_I(b[0], 0);
}
TC_CASE(already_converted)
{
    char a[16] = "abc 123 _.", c[16] = "ABC 123 _.";
    _strlwr(a); CHECK_STR(a, "abc 123 _.");
    _strupr(c); CHECK_STR(c, "ABC 123 _.");
}
TC_CASE(high_bytes)
{
    char b[8]; int i;
    b[0] = 'A'; b[1] = (char)0xC0; b[2] = (char)0xE9; b[3] = (char)0x80; b[4] = (char)0xFF; b[5] = 'z'; b[6] = 0;
    _strlwr(b);
    CHECK_EQ_I((unsigned char)b[0], 'a'); CHECK_EQ_I((unsigned char)b[1], 0xC0); CHECK_EQ_I((unsigned char)b[2], 0xE9);
    CHECK_EQ_I((unsigned char)b[3], 0x80); CHECK_EQ_I((unsigned char)b[4], 0xFF); CHECK_EQ_I((unsigned char)b[5], 'z');
    _strupr(b);
    CHECK_EQ_I((unsigned char)b[0], 'A'); CHECK_EQ_I((unsigned char)b[1], 0xC0); CHECK_EQ_I((unsigned char)b[2], 0xE9);
    CHECK_EQ_I((unsigned char)b[3], 0x80); CHECK_EQ_I((unsigned char)b[4], 0xFF); CHECK_EQ_I((unsigned char)b[5], 'Z');
    i = 0; while (b[i]) i++; CHECK_EQ_I(i, 6);
}
TC_CASE(length_and_guard)
{
    char b[12]; int i;
    for (i = 0; i < 12; i++) b[i] = 'G';
    b[0] = 'a'; b[1] = 'B'; b[2] = 'c'; b[3] = 0;
    _strupr(b); CHECK_STR(b, "ABC"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'G'); CHECK_EQ_I(b[11], 'G');
    _strlwr(b); CHECK_STR(b, "abc"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'G');
}
int main(void) { tc_begin(); TC_RUN(mixed); TC_RUN(empty); TC_RUN(already_converted); TC_RUN(high_bytes); TC_RUN(length_and_guard); return tc_end(); }
