/* TC
 * id:    hp_neg_aligned_msize_wrong_alignment_probe
 * area:  heap
 * funcs: _aligned_msize _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_msize (alignment and offset must match those used at allocation; mismatch is undefined)
 * note:  PROBE: _aligned_msize with a valid power-of-two alignment (16, 128) different from the allocation's 64. Documented
 *        behaviour is "must match", so the result may be garbage; only recorded (TC INFO), nothing asserted beyond no crash.
 *        The matching-alignment control is asserted: _aligned_msize(p,64,0) >= 100.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_100 = 100, v_64 = 64, v_16 = 16, v_128 = 128, v_0 = 0;

static void info(const char *k, long long v)
{
    char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}
TC_CASE(control_matching)
{
    void *p = _aligned_malloc(v_100, v_64);
    CHECK(p != NULL); if (!p) return;
    CHECK(_aligned_msize(p, v_64, v_0) >= 100);
    _aligned_free(p);
}
TC_CASE(wrong_alignments)
{
    void *p; size_t s;
    tc_iph_install();
    p = _aligned_malloc(v_100, v_64);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    s = _aligned_msize(p, v_16, v_0);
    info("msize_al16", (long long)s); info("al16_errno", errno); info("al16_iph", tc_iph_count);
    tc_iph_count = 0; errno = 0;
    s = _aligned_msize(p, v_128, v_0);
    info("msize_al128", (long long)s); info("al128_errno", errno); info("al128_iph", tc_iph_count);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_matching); TC_RUN(wrong_alignments);
    return tc_end();
}
