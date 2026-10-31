/* TC
 * id:    mem_memcmp_alignment_sizes
 * area:  memory
 * funcs: memcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.1; MS docs memcmp
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static unsigned char A[16 + 140], B[16 + 140];
static volatile size_t v_n;
#define SGN(x) (((x) > 0) - ((x) < 0))
static int cmp(const void *a, const void *b, size_t n) { v_n = n; return memcmp(a, b, v_n); }
static void init(void) { size_t i; for (i = 0; i < sizeof A; i++) A[i] = B[i] = (unsigned char)(i * 7 + 3); }

TC_CASE(equal_all_sizes)
{
    size_t a, n; int bad = 0; init();
    for (a = 0; a < 16; a++) for (n = 0; n <= 130; n++) if (cmp(A + a, B + a, n) != 0) bad++;
    CHECK_EQ_I(bad, 0);
}
TC_CASE(diff_at_every_index_sign_lt)
{
    size_t a, i; int bad = 0; const size_t n = 130;
    for (a = 0; a < 16; a++) for (i = 0; i < n; i++) {
        init(); A[a + i] = 0x01; B[a + i] = 0xf0;
        if (SGN(cmp(A + a, B + a, n)) != -1) bad++;
        if (cmp(A + a, B + a, i) != 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(diff_at_every_index_sign_gt)
{
    size_t a, i; int bad = 0; const size_t n = 130;
    for (a = 0; a < 16; a++) for (i = 0; i < n; i++) {
        init(); A[a + i] = 0xf0; B[a + i] = 0x01;
        if (SGN(cmp(A + a, B + a, n)) != 1) bad++;
        if (cmp(A + a, B + a, i) != 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(misaligned_pairs)
{
    size_t a, b, i; int bad = 0; const size_t n = 100;
    init();
    for (a = 0; a < 16; a++) for (b = 0; b < 16; b++) {
        for (i = 0; i < n; i++) B[b + i] = A[a + i];
        if (cmp(A + a, B + b, n) != 0) bad++;
        B[b + n - 1] ^= 0x80;
        if (cmp(A + a, B + b, n) == 0) bad++;
        if (cmp(A + a, B + b, n - 1) != 0) bad++;
    }
    CHECK_EQ_I(bad, 0);
}

int main(void) { tc_begin(); TC_RUN(equal_all_sizes); TC_RUN(diff_at_every_index_sign_lt); TC_RUN(diff_at_every_index_sign_gt); TC_RUN(misaligned_pairs); return tc_end(); }
