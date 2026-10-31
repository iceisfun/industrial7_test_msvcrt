/* TC
 * id:    mem_memcmp_unsigned_bytes
 * area:  memory
 * funcs: memcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4 (compared as unsigned char); MS docs memcmp
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_n;
#define SGN(x) (((x) > 0) - ((x) < 0))
static int cmp(const void *a, const void *b, size_t n) { v_n = n; return memcmp(a, b, v_n); }

TC_CASE(0x80_gt_0x7f) { static const unsigned char a[] = {0x80}, b[] = {0x7f}; CHECK_EQ_I(SGN(cmp(a, b, 1)), 1); CHECK_EQ_I(SGN(cmp(b, a, 1)), -1); }
TC_CASE(c_0xff_gt_0x00) { static const unsigned char a[] = {0xff}, b[] = {0x00}; CHECK_EQ_I(SGN(cmp(a, b, 1)), 1); }
TC_CASE(c_0x00_lt_0xff) { static const unsigned char a[] = {0x00}, b[] = {0xff}; CHECK_EQ_I(SGN(cmp(a, b, 1)), -1); }
TC_CASE(high_bit_embedded_nul)
{
    static const unsigned char a[] = {0x01, 0x00, 0x80, 0x00}, b[] = {0x01, 0x00, 0x7f, 0xff};
    CHECK_EQ_I(SGN(cmp(a, b, 4)), 1);
    CHECK_EQ_I(SGN(cmp(b, a, 4)), -1);
}

int main(void) { tc_begin(); TC_RUN(0x80_gt_0x7f); TC_RUN(c_0xff_gt_0x00); TC_RUN(c_0x00_lt_0xff); TC_RUN(high_bit_embedded_nul); return tc_end(); }
