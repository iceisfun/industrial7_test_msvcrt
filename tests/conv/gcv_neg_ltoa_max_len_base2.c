/* TC
 * id:    gcv_neg_ltoa_max_len_base2
 * area:  conv
 * funcs: _ltoa
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa/_ltoa: buffer must hold up to 33 bytes for _ltoa (base 2, 32 digits + NUL)
 * note:  Worst-case output must not overrun the documented 33-byte size.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile long lv;
static volatile int rad;

TC_CASE(minus1_base2_buf33)
{
    struct { char b[33]; char g[8]; } s; int i;
    for (i = 0; i < 33; i++) s.b[i] = 'X';
    for (i = 0; i < 8; i++) s.g[i] = (char)0xA5;
    lv = -1; rad = 2; _ltoa(lv, s.b, rad);
    for (i = 0; i < 32; i++) CHECK_EQ_I(s.b[i], '1');
    CHECK_EQ_I(s.b[32], 0);
    for (i = 0; i < 8; i++) CHECK_EQ_U((unsigned char)s.g[i], 0xA5);
}
TC_CASE(min_base2)
{
    char b[34]; int i;
    for (i = 0; i < 34; i++) b[i] = 'X';
    lv = LONG_MIN; rad = 2; _ltoa(lv, b, rad);
    CHECK_EQ_I(b[0], '1');
    for (i = 1; i < 32; i++) CHECK_EQ_I(b[i], '0');
    CHECK_EQ_I(b[32], 0);
    CHECK_EQ_I(b[33], 'X');
}
TC_CASE(minus1_dec_guard)
{
    char b[34]; int i;
    for (i = 0; i < 34; i++) b[i] = 'X';
    lv = -1; rad = 10; _ltoa(lv, b, rad);
    CHECK_STR(b, "-1");
    for (i = 3; i < 34; i++) CHECK_EQ_I(b[i], 'X');
}

int main(void) { tc_begin(); TC_RUN(minus1_base2_buf33); TC_RUN(min_base2); TC_RUN(minus1_dec_guard); return tc_end(); }
