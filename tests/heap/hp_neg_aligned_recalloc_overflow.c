/* TC
 * id:    hp_neg_aligned_recalloc_overflow
 * area:  heap
 * kind:  negative
 * funcs: _aligned_recalloc _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p2
 * conf:  inferred
 * ref:   MS docs _aligned_recalloc (overflowing num*size -> NULL; alignment must be power of 2 -> EINVAL via invalid-parameter handler)
 * note:  The ENOMEM errno on overflow is not documented for _aligned_recalloc; it is probed separately in
 *        hp_neg_aligned_recalloc_overflow_errno_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_num, v_size, v_al;

TC_CASE(count_size_overflow)
{
    void *p;
    v_num = SIZE_MAX; v_size = 2; v_al = 16; errno = 0;
    p = _aligned_recalloc(NULL, v_num, v_size, v_al);
    CHECK(p == NULL);
    if (p) _aligned_free(p);
}
TC_CASE(overflow_keeps_old_block)
{
    unsigned char *p = (unsigned char *)_aligned_malloc(32, 16), *q;
    CHECK(p != NULL);
    if (!p) return;
    p[0] = 0x5a; p[31] = 0xa5;
    v_num = SIZE_MAX / 2 + 1; v_size = 2; v_al = 16; errno = 0;
    q = (unsigned char *)_aligned_recalloc(p, v_num, v_size, v_al);
    CHECK(q == NULL);
    if (q) { _aligned_free(q); return; }
    CHECK_EQ_I(p[0], 0x5a);
    CHECK_EQ_I(p[31], 0xa5);
    _aligned_free(p);
}
TC_CASE(bad_alignment)
{
    void *p;
    CHECK(tc_iph_install());
    v_num = 4; v_size = 4; v_al = 3; errno = 0; tc_iph_count = 0;
    p = _aligned_recalloc(NULL, v_num, v_size, v_al);
    CHECK(p == NULL);
    if (p) { _aligned_free(p); return; }
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(count_size_overflow); TC_RUN(overflow_keeps_old_block); TC_RUN(bad_alignment); return tc_end(); }
