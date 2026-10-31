/* TC
 * id:    hp_neg_aligned_nomem_errno_probe
 * area:  heap
 * funcs: _aligned_malloc _aligned_offset_malloc _aligned_realloc _aligned_recalloc _aligned_offset_recalloc _aligned_free
 * procs: _malloca _freea
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc / _aligned_realloc / _aligned_recalloc / _malloca (errno on out-of-memory not documented per page)
 * note:  PROBE: errno after allocation failure. Expected ENOMEM (these CRT functions sit on malloc, which documents ENOMEM) for
 *        _aligned_malloc/_aligned_offset_malloc/_aligned_realloc/_aligned_recalloc with SIZE_MAX/2 bytes and for the exported
 *        _malloca(SIZE_MAX-64). Also probes count*size overflow in _aligned_offset_recalloc (expected NULL, block kept).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

typedef void *(__cdecl *malloca_t)(size_t);
typedef void (__cdecl *freea_t)(void *);
static volatile size_t v_huge = SIZE_MAX / 2, v_near = SIZE_MAX - 64, v_16 = 16, v_8 = 8, v_32 = 32, v_1 = 1, v_0 = 0;

TC_CASE(aligned_malloc) { void *p; errno = 0; p = _aligned_malloc(v_huge, v_16); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); if (p) _aligned_free(p); }
TC_CASE(aligned_offset_malloc) { void *p; errno = 0; p = _aligned_offset_malloc(v_huge, v_16, v_8); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); if (p) _aligned_free(p); }
TC_CASE(aligned_realloc)
{
    void *p, *r = NULL;
    p = _aligned_malloc(v_32, v_32); CHECK(p != NULL); if (!p) return;
    errno = 0; r = _aligned_realloc(p, v_huge, v_32);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOMEM);
    _aligned_free(r ? r : p);
}
TC_CASE(aligned_recalloc)
{
    void *p, *r = NULL;
    p = _aligned_malloc(v_32, v_32); CHECK(p != NULL); if (!p) return;
    errno = 0; r = _aligned_recalloc(p, v_1, v_huge, v_32);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOMEM);
    _aligned_free(r ? r : p);
    errno = 0; r = _aligned_recalloc(NULL, v_1, v_huge, v_16);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOMEM);
    if (r) _aligned_free(r);
}
TC_CASE(offset_recalloc_count_size_overflow)
{
    void *p, *r = NULL;
    volatile size_t cnt = SIZE_MAX / 2, sz = 4;
    tc_iph_install();
    p = _aligned_malloc(v_32, v_16); CHECK(p != NULL); if (!p) return;
    r = _aligned_offset_recalloc(p, cnt, sz, v_16, v_0);
    CHECK(r == NULL);
    _aligned_free(r ? r : p);
}
TC_CASE(malloca_near_max)
{
    malloca_t ma = (malloca_t)tc_crt_proc("_malloca"); freea_t fa = (freea_t)tc_crt_proc("_freea");
    void *p;
    if (!ma) TC_SKIP("no _malloca");
    errno = 0; p = ma(v_near);
    CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM);
    if (p && fa) fa(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(aligned_malloc); TC_RUN(aligned_offset_malloc); TC_RUN(aligned_realloc); TC_RUN(aligned_recalloc);
    TC_RUN(offset_recalloc_count_size_overflow); TC_RUN(malloca_near_max);
    return tc_end();
}
