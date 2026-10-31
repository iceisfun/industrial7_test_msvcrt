/* TC
 * id:    hp_neg_aligned_malloc_huge
 * area:  heap
 * funcs: _aligned_malloc
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _aligned_malloc
 * note:  Overflow of size+alignment+pointer slot must be detected; errno ENOMEM or EINVAL accepted. The wrap-around cases are only asserted for msvcr100+ (older CRTs unknown); errno only checked for msvcr100+ (msvcrt.dll malloc does not document ENOMEM).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

#ifndef ENOMEM
#define ENOMEM 12
#endif
TC_CASE(size_max)
{
    volatile size_t sz = SIZE_MAX, a = 16; void *p;
    if (!TC_AT_LEAST(100)) TC_SKIP("overflow check unknown before msvcr100");
    tc_iph_install();
    errno = 0;
    p = _aligned_malloc(sz, a);
    CHECK(p == NULL);
    CHECK(errno == ENOMEM || errno == EINVAL);
    if (p) _aligned_free(p);
}
TC_CASE(size_max_minus_8)
{
    volatile size_t sz = SIZE_MAX - 8, a = 64; void *p;
    if (!TC_AT_LEAST(100)) TC_SKIP("overflow check unknown before msvcr100");
    tc_iph_install();
    errno = 0;
    p = _aligned_malloc(sz, a);
    CHECK(p == NULL);
    CHECK(errno == ENOMEM || errno == EINVAL);
    if (p) _aligned_free(p);
}
TC_CASE(one_shl_42)
{
    volatile size_t sz = TC_X64 ? ((size_t)1 << 42) : 0; void *p;
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_install();
    errno = 0;
    p = _aligned_malloc(sz, 16);
    CHECK(p == NULL);
    if (TC_AT_LEAST(100)) CHECK(errno == ENOMEM || errno == EINVAL);
    if (p) _aligned_free(p);
}
/* 64 TB: fails instantly on Windows but wedged the Wine runner for minutes (and contributed to a host memory flatline) */
TC_CASE(one_shl_46)
{
    volatile size_t sz = TC_X64 ? ((size_t)1 << 46) : 0; void *p;
    TC_HAZARD("1<<46 aligned_malloc stalls Wine");
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_install();
    errno = 0;
    p = _aligned_malloc(sz, 16);
    CHECK(p == NULL);
    if (TC_AT_LEAST(100)) CHECK(errno == ENOMEM || errno == EINVAL);  /* older CRTs: errno on heap failure not documented */
    if (p) _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(size_max); TC_RUN(size_max_minus_8); TC_RUN(one_shl_42); TC_RUN(one_shl_46);
    return tc_end();
}
