/* TC
 * id:    stl_swab_basic
 * area:  stdlib
 * funcs: _swab
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swab: copies n bytes from src to dest, swapping each pair of adjacent bytes
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static void sw(const char *s, char *d, int n) { _swab((char *)s, d, n); }

TC_CASE(abcd)
{
    char d[8]; memset(d, 'x', sizeof d); d[5] = 0;
    sw("abcd", d, 4);
    CHECK_MEM(d, "badcx", 5);
}
TC_CASE(n2)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    sw("abcd", d, 2);
    CHECK_MEM(d, "baxx", 4);
}
TC_CASE(n0_untouched)
{
    char d[4] = { 'q', 'r', 's', 't' };
    sw("abcd", d, 0);
    CHECK_MEM(d, "qrst", 4);
}
TC_CASE(guard_bytes)
{
    unsigned char buf[8]; int i;
    for (i = 0; i < 8; i++) buf[i] = 0xEE;
    sw("1234", (char *)buf + 2, 4);
    CHECK_EQ_U(buf[0], 0xEE); CHECK_EQ_U(buf[1], 0xEE);
    CHECK_MEM(buf + 2, "2143", 4);
    CHECK_EQ_U(buf[6], 0xEE); CHECK_EQ_U(buf[7], 0xEE);
}
TC_CASE(nul_and_high_bytes)
{
    static const unsigned char src[6] = { 0x00, 0xFF, 0x80, 0x00, 0x7F, 0xFE };
    static const unsigned char want[6] = { 0xFF, 0x00, 0x00, 0x80, 0xFE, 0x7F };
    unsigned char d[6] = { 1, 1, 1, 1, 1, 1 };
    _swab((char *)src, (char *)d, 6);
    CHECK_MEM(d, want, 6);
}
TC_CASE(six)
{
    char d[7]; memset(d, 0, sizeof d);
    sw("ABCDEF", d, 6);
    CHECK_STR(d, "BADCFE");
}
int main(void)
{
    tc_begin();
    TC_RUN(abcd); TC_RUN(n2); TC_RUN(n0_untouched); TC_RUN(guard_bytes); TC_RUN(nul_and_high_bytes); TC_RUN(six);
    return tc_end();
}
