/* TC
 * id:    hp_neg_aligned_offset_recalloc_bad_args
 * area:  heap
 * funcs: _aligned_offset_recalloc _aligned_offset_malloc _aligned_free
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_recalloc (alignment power of 2, offset < count*size; invalid parameter -> EINVAL, NULL)
 * note:  The count*size overflow case moved to hp_neg_aligned_nomem_errno_probe (behaviour not documented).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_one = 1, v_16 = 16, v_32 = 32, v_64 = 64, v_3 = 3, v_100 = 100, v_0 = 0, v_63 = 63;

static unsigned char *mk(void)
{
    unsigned char *p = (unsigned char *)_aligned_offset_malloc(v_32, v_16, v_0); int i;
    if (p) for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 1);
    return p;
}
static void intact(unsigned char *p) { int i; for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], i + 1); }

TC_CASE(bad_alignment)
{
    unsigned char *p = mk(), *r;
    CHECK(tc_iph_install());
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = (unsigned char *)_aligned_offset_recalloc(p, v_one, v_64, v_3, v_0);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (r) { _aligned_free(r); return; }
    intact(p); _aligned_free(p);
}
TC_CASE(offset_ge_size)
{
    unsigned char *p = mk(), *r;
    tc_iph_install();
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = (unsigned char *)_aligned_offset_recalloc(p, v_one, v_64, v_16, v_100);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (r) { _aligned_free(r); return; }
    intact(p); _aligned_free(p);
}
TC_CASE(offset_equal_boundary)
{
    unsigned char *p = mk(), *r;
    tc_iph_install();
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = (unsigned char *)_aligned_offset_recalloc(p, v_one, v_64, v_16, v_64);   /* offset == count*size */
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    if (r) { _aligned_free(r); return; }
    intact(p);
    tc_iph_count = 0;
    r = (unsigned char *)_aligned_offset_recalloc(p, v_one, v_64, v_16, v_63);   /* offset == count*size-1 */
    CHECK(r != NULL); CHECK_EQ_I(tc_iph_count, 0);
    if (r) { CHECK_EQ_U(((uintptr_t)r + 63) % 16, 0); _aligned_free(r); }
    else _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(bad_alignment); TC_RUN(offset_ge_size); TC_RUN(offset_equal_boundary);
    return tc_end();
}
