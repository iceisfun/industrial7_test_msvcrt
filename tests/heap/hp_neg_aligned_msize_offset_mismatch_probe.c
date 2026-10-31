/* TC
 * id:    hp_neg_aligned_msize_offset_mismatch_probe
 * area:  heap
 * kind:  negative
 * funcs: _aligned_offset_malloc _aligned_free _aligned_msize
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _aligned_msize / _aligned_offset_malloc
 * note:  PROBING what _aligned_msize does when passed an offset that differs from the allocation offset (63 instead of 8).
 *        Only the matching-offset control is asserted; the mismatch outcome is merely required to be survivable and is recorded on stdout.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>

static volatile size_t v_off = 63;

TC_CASE(matching_offset_control)
{
    void *p = _aligned_offset_malloc(64, 16, 8); size_t r;
    CHECK(p != NULL);
    if (!p) return;
    CHECK_EQ_U((size_t)(((size_t)p + 8) & 15), 0);
    r = _aligned_msize(p, 16, 8);
    CHECK(r >= 64);
    _aligned_free(p);
}
TC_CASE(mismatched_offset_recorded)
{
    void *p = _aligned_offset_malloc(64, 16, 8); size_t r; char b[32];
    CHECK(p != NULL);
    if (!p) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, 16, v_off);
    tc_write("TC NOTE mismatch result=");
    tc_i64((long long)r, b); tc_write(b);
    tc_write(" iph="); tc_i64(tc_iph_count, b); tc_write(b);
    tc_write(" errno="); tc_i64(errno, b); tc_write(b); tc_write("\n");
    CHECK(tc_iph_count == 0 || tc_iph_count == 1);
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(matching_offset_control); TC_RUN(mismatched_offset_recorded); return tc_end(); }
