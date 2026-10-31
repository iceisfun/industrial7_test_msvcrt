/* TC
 * id:    hp_neg_aligned_realloc_zero_alignment_probe
 * area:  heap
 * funcs: _aligned_realloc _aligned_offset_realloc _aligned_malloc _aligned_free _aligned_msize
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_realloc ("alignment must be an integer power of 2"; 0 is not documented either way)
 * note:  PROBE: alignment == 0 is treated by the CRT's power-of-two check (x & (x-1)) == 0 as valid or invalid? Windows may
 *        accept 0 (clamped to the minimum alignment) or reject with EINVAL + invalid-parameter handler. Results are printed as
 *        TC INFO lines. Asserted: the program does not crash and a returned block can be freed. A failed realloc leaves p valid.
 *        _aligned_msize(p,0,0) is invoked under the handler; the value is recorded only.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_0 = 0, v_64 = 64, v_32 = 32;

static void info(const char *k, long long v)
{
    char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}
TC_CASE(realloc_zero_align)
{
    void *p, *r;
    tc_iph_install();
    p = _aligned_malloc(v_32, v_32);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = _aligned_realloc(p, v_64, v_0);
    info("realloc_null", r == NULL); info("realloc_errno", errno); info("realloc_iph", tc_iph_count);
    _aligned_free(r ? r : p);
}
TC_CASE(offset_realloc_null_zero_align)
{
    void *r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _aligned_offset_realloc(NULL, v_64, v_0, v_0);
    info("ofsrealloc_null", r == NULL); info("ofsrealloc_errno", errno); info("ofsrealloc_iph", tc_iph_count);
    if (r) _aligned_free(r);
}
TC_CASE(msize_zero_align)
{
    void *p; size_t s;
    tc_iph_install();
    p = _aligned_malloc(v_64, v_32);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    s = _aligned_msize(p, v_0, v_0);
    info("msize", (long long)s); info("msize_errno", errno); info("msize_iph", tc_iph_count);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(realloc_zero_align); TC_RUN(offset_realloc_null_zero_align); TC_RUN(msize_zero_align);
    return tc_end();
}
