/* TC
 * id:    mem_neg_memcpy_s_aliasing_same_ptr_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s (overlapping buffers: behaviour undefined; use memmove_s)
 * note:  PROBE: memcpy_s(p, n, p, n) with identical pointers. Likely returns 0 leaving the buffer unchanged. Hard assertions are
 *        lenient: return is 0 or EINVAL/ERANGE; errno matches a nonzero return; buffer is unchanged (ret 0) or unchanged-or-zeroed (failure).
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_cpy;

TC_CASE(same_pointer)
{
    unsigned char b[8], o[8], z[8] = {0}; volatile size_t n = 8; int i, r, same, zero; char t[24];
    CHECK(p_cpy != NULL); if (!p_cpy) return;
    for (i = 0; i < 8; i++) b[i] = o[i] = (unsigned char)(0x10 + i);
    tc_iph_count = 0; errno = 0;
    r = p_cpy(b, n, b, n);
    tc_write("TC NOTE " TC_TEST_ID " ret="); tc_i64(r, t); tc_write(t);
    tc_write(" iph="); tc_i64(tc_iph_count, t); tc_write(t); tc_write("\n");
    for (same = 1, zero = 1, i = 0; i < 8; i++) { if (b[i] != o[i]) same = 0; if (b[i] != z[i]) zero = 0; }
    CHECK(r == 0 || r == EINVAL || r == ERANGE);
    if (r) CHECK_EQ_I(errno, r);
    if (r == 0) CHECK(same);
    CHECK(same || zero);
}
int main(void)
{
    tc_begin();
    p_cpy = (mcs_t)tc_crt_proc("memcpy_s");
    tc_iph_install();
    TC_RUN(same_pointer);
    return tc_end();
}
