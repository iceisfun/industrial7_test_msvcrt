/* TC
 * id:    hp_neg_aligned_malloc_bad_args
 * area:  heap
 * kind:  negative
 * funcs:
 * procs: _aligned_malloc _aligned_offset_malloc _aligned_free
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _aligned_malloc, _aligned_offset_malloc (alignment must be a power of 2; offset must be < size; invalid parameter -> EINVAL, NULL)
 * note:  runs with a counting invalid-parameter handler installed.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

typedef void *(__cdecl *am_t)(size_t, size_t);
typedef void *(__cdecl *aom_t)(size_t, size_t, size_t);
typedef void (__cdecl *af_t)(void *);
static am_t p_am; static aom_t p_aom; static af_t p_af;

TC_CASE(non_power_of_two)
{
    volatile size_t al = 24; void *volatile p;
    if (!p_am || !p_aom || !p_af || !tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    p = p_am(64, al);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    p_af(p);
}
TC_CASE(three)
{
    volatile size_t al = 3; void *volatile p;
    if (!p_am || !p_aom || !p_af || !tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    p = p_am(16, al);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(offset_ge_size)
{
    volatile size_t off = 32; void *volatile p;
    if (!p_am || !p_aom || !p_af || !tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    p = p_aom(16, 16, off);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); p_am=(am_t)tc_crt_proc("_aligned_malloc"); p_aom=(aom_t)tc_crt_proc("_aligned_offset_malloc"); p_af=(af_t)tc_crt_proc("_aligned_free"); TC_RUN(non_power_of_two); TC_RUN(three); TC_RUN(offset_ge_size); return tc_end(); }
