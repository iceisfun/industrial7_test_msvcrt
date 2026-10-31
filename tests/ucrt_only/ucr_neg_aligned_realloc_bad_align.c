/* TC
 * id:    ucr_neg_aligned_realloc_bad_align
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_realloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_realloc (invalid alignment: invalid parameter handler, EINVAL, NULL)
 * note:  original-block-preserved assertion moved to ucr_neg_aligned_realloc_keeps_block_probe
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a, g_s;
TC_CASE(align3)
{
    unsigned char *p, *q; size_t i; int b = tc_iph_count;
    g_s = 64; g_a = 16; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(i + 1);
    g_s = 256; g_a = 3; errno = 0;
    q = _aligned_realloc(p, g_s, g_a);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - b, 1);
    if (q) { _aligned_free(q); return; }
    (void)i;
    _aligned_free(p);
}
TC_CASE(null_block_align3)
{
    void *q; int b = tc_iph_count;
    g_s = 64; g_a = 3; errno = 0;
    q = _aligned_realloc(NULL, g_s, g_a);
    CHECK(q == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - b, 1);
    if (q) _aligned_free(q);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 1; TC_RUN(align3); TC_RUN(null_block_align3); return tc_end(); }
