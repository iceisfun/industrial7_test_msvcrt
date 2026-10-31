/* TC
 * id:    gcv_ltoa_basic
 * area:  conv
 * funcs: _ltoa
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _itoa, _ltoa, _ultoa (radix 10 negative gets '-'; otherwise value treated as unsigned)
 * note:  Dedicated _ltoa grid with exact-size buffers and guard bytes.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile long lv;
static volatile int rad;

/* buffer of exactly strlen(expect)+1 followed by guard bytes */
static void chk(long v, int r, const char *expect)
{
    struct { char b[40]; } g; unsigned n = 0, i; char *ret; const char *e = expect;
    while (e[n]) n++;
    for (i = 0; i < sizeof g.b; i++) g.b[i] = 0x55;
    lv = v; rad = r;
    ret = _ltoa(lv, g.b, rad);
    CHECK_EQ_P(ret, g.b);
    CHECK_STR(g.b, expect);
    for (i = n + 1; i < sizeof g.b; i++) if ((unsigned char)g.b[i] != 0x55) { CHECK(!"guard byte overwritten"); break; }
}

TC_CASE(decimal) { chk(0, 10, "0"); chk(123456, 10, "123456"); chk(-123456, 10, "-123456"); }
TC_CASE(limits) { chk(LONG_MAX, 10, "2147483647"); chk(LONG_MIN, 10, "-2147483648"); }
TC_CASE(bases) { chk(255, 16, "ff"); chk(5, 2, "101"); chk(64, 8, "100"); chk(35, 36, "z"); chk(36, 36, "10"); }
TC_CASE(neg_unsigned) { chk(-1, 16, "ffffffff"); chk(-1, 2, "11111111111111111111111111111111"); chk(-1, 8, "37777777777"); }
TC_CASE(retval) { char b[34]; lv = 9; rad = 10; CHECK_EQ_P(_ltoa(lv, b, rad), b); CHECK_STR(b, "9"); }

int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(limits); TC_RUN(bases); TC_RUN(neg_unsigned); TC_RUN(retval); return tc_end(); }
