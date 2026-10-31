/* TC
 * id:    ucr_neg_o_memcpy_s_zeroes_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_memcpy_s
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs memcpy_s
 * note:  Also probes that _itoa_s sets buffer[0]=0 on ERANGE (separate case).
 *        Probes that on ERANGE (count > destsize) the destination buffer is zero-filled by ucrtbase.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *memcpy_s_t)(void *, size_t, const void *, size_t);

TC_CASE(dest_zeroed)
{
    memcpy_s_t f = (memcpy_s_t)tc_crt_proc("_o_memcpy_s"); char d[4] = {'x', 'x', 'x', 'x'}; static const char s[8] = "abcdefg"; static const char z[4] = {0};
    if (!f) TC_SKIP("missing");
    tc_iph_count = 0;
    CHECK_EQ_I(f(d, sizeof d, s, 8), ERANGE); CHECK_MEM(d, z, 4);
}
TC_CASE(itoa_s_small_empties)
{
    typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
    itoa_s_t f = (itoa_s_t)tc_crt_proc("_o__itoa_s"); char b[2] = {'x', 'x'};
    if (!f) TC_SKIP("missing");
    tc_iph_count = 0;
    CHECK_EQ_I(f(12345, b, 2, 10), ERANGE); CHECK_EQ_I(b[0], 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(dest_zeroed); TC_RUN(itoa_s_small_empties); return tc_end(); }
