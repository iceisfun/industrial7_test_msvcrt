/* TC
 * id:    mem_neg_memcpy_s_zeroing_probe
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=80
 * tier:  p0
 * conf:  probe
 * ref:   MS docs memcpy_s
 * note:  PROBE: on failure (src NULL, or count > destSize) real memcpy_s fills the whole destination with zero (believed from the CRT implementation); memmove_s is believed NOT to modify dest on failure.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *cpy_s_t)(void *, size_t, const void *, size_t);
static cpy_s_t p_memcpy_s, p_memmove_s;
static volatile size_t v_n;

TC_CASE(src_null_zeroes_dest)
{
    unsigned char d[8]; size_t i, nz = 0;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); memset(d, 0x5a, 8); v_n = 8;
    p_memcpy_s(d, 8, NULL, v_n);
    for (i = 0; i < 8; i++) nz += d[i] != 0;
    CHECK_EQ_U(nz, 0);
}
TC_CASE(too_small_zeroes_dest)
{
    unsigned char d[4], s[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; size_t i, nz = 0;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); memset(d, 0x5a, 4); v_n = 8;
    p_memcpy_s(d, 4, s, v_n);
    for (i = 0; i < 4; i++) nz += d[i] != 0;
    CHECK_EQ_U(nz, 0);
}
TC_CASE(memmove_s_leaves_dest)
{
    unsigned char d[4], s[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; size_t i, ch = 0;
    if (!p_memmove_s) TC_SKIP("memmove_s missing");
    tc_iph_install(); memset(d, 0x5a, 4); v_n = 8;
    p_memmove_s(d, 4, s, v_n);
    for (i = 0; i < 4; i++) ch += d[i] != 0x5a;
    CHECK_EQ_U(ch, 0);
}
int main(void)
{
    tc_begin(); p_memcpy_s = (cpy_s_t)tc_crt_proc("memcpy_s"); p_memmove_s = (cpy_s_t)tc_crt_proc("memmove_s");
    TC_RUN(src_null_zeroes_dest); TC_RUN(too_small_zeroes_dest); TC_RUN(memmove_s_leaves_dest);
    return tc_end();
}
