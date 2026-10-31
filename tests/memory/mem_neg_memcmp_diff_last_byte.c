/* TC
 * id:    mem_neg_memcmp_diff_last_byte
 * area:  memory
 * kind:  negative
 * funcs: memcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.1 memcmp; MS docs memcmp
 * note:  memcmp must not report equality when only the last compared byte differs (catches vectorised tail bugs).
 */
#include "tc.h"
#include <string.h>

static unsigned char A[512] __attribute__((aligned(64))), B[512] __attribute__((aligned(64)));

static void fill(void) { int i; for (i = 0; i < 512; i++) A[i] = B[i] = (unsigned char)(i * 7 + 3); }

TC_CASE(sizes_1_to_130_last_byte_differs_nonzero)
{
    size_t n, off;
    for (off = 0; off < 3; off++)
    for (n = 1; n <= 130; n++) {
        volatile size_t vn = n;
        fill();
        A[off + n - 1] = 0x10; B[off + n - 1] = 0x11;
        if (!(memcmp(A + off, B + off, vn) < 0)) { CHECK_EQ_U(n, 0); CHECK_EQ_U(off, 99); break; }
        if (!(memcmp(B + off, A + off, vn) > 0)) { CHECK_EQ_U(n, 1); CHECK_EQ_U(off, 98); break; }
        /* one byte shorter must compare equal */
        if (n > 1 && memcmp(A + off, B + off, vn - 1) != 0) { CHECK_EQ_U(n, 2); CHECK_EQ_U(off, 97); break; }
    }
}
TC_CASE(single_bit_diff_nonzero)
{
    size_t n; int bit;
    for (n = 1; n <= 64; n += 3)
        for (bit = 0; bit < 8; bit++) {
            volatile size_t vn = n;
            fill();
            B[n - 1] = (unsigned char)(A[n - 1] ^ (1 << bit));
            if (memcmp(A, B, vn) == 0) { CHECK_EQ_U(n, 0); CHECK_EQ_U(bit, 99); return; }
        }
}
TC_CASE(diff_at_n_minus_1_with_aligned_head)
{
    size_t n;
    for (n = 16; n <= 256; n += 16) {
        volatile size_t vn = n;
        fill();
        B[n - 1] ^= 0x80;
        if (memcmp(A, B, vn) == 0) { CHECK_EQ_U(n, 0); break; }
        if (memcmp(A, B, vn - 1) != 0) { CHECK_EQ_U(n, 1); break; }
    }
}
int main(void) { tc_begin(); TC_RUN(sizes_1_to_130_last_byte_differs_nonzero); TC_RUN(single_bit_diff_nonzero); TC_RUN(diff_at_n_minus_1_with_aligned_head); return tc_end(); }
