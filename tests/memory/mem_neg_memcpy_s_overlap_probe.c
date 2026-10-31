/* TC
 * id:    mem_neg_memcpy_s_overlap_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s: overlapping buffers undefined
 * note:  PROBE: docs say overlap is undefined; probes whether implementation acts as memmove and returns 0 with no handler call.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(overlap_dest_below_src_returns_0_and_memmove_result)
{
    char b[8] = {'0', '1', '2', '3', '4', '5', '6', '7'}; volatile size_t n = 6;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(b, 8, b + 2, n), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_MEM(b, "23456767", 8);
}
TC_CASE(overlap_dest_above_src_returns_0_and_memmove_result)
{
    char b[8] = {'0', '1', '2', '3', '4', '5', '6', '7'}; volatile size_t n = 6;
    if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(b + 2, 6, b, n), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_MEM(b, "01012345", 8);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(overlap_dest_below_src_returns_0_and_memmove_result); TC_RUN(overlap_dest_above_src_returns_0_and_memmove_result);
    return tc_end();
}
