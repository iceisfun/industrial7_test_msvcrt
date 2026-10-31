/* TC
 * id:    hp_neg_malloca_huge
 * area:  heap
 * funcs:
 * procs: _malloca _freea
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _malloca / _freea (heap path for large sizes; _freea(NULL) is a no-op)
 * note:  Uses the exported _malloca/_freea functions (via GetProcAddress) because mingw's header macro would wrap
 *        SIZE_MAX+16 around and take the stack path. Only NULL is asserted for SIZE_MAX (size+marker overflows).
 *        errno for _malloca failure is probed in hp_neg_aligned_nomem_errno_probe.
 */
#include "tc.h"
#include <errno.h>
#include <stdint.h>

typedef void *(__cdecl *malloca_t)(size_t);
typedef void (__cdecl *freea_t)(void *);
static malloca_t p_malloca; static freea_t p_freea;
static volatile size_t v_max = SIZE_MAX, v_near = SIZE_MAX - 64, v_small = 64;

TC_CASE(exported) { CHECK(p_malloca != NULL); CHECK(p_freea != NULL); }
TC_CASE(size_max)
{
    void *p;
    if (!p_malloca) TC_SKIP("no _malloca");
    tc_iph_install(); tc_iph_count = 0;
    p = p_malloca(v_max);
    CHECK(p == NULL);
    if (p && p_freea) p_freea(p);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(freea_null_and_normal)
{
    unsigned char *p; int i;
    if (!p_malloca || !p_freea) TC_SKIP("no _malloca/_freea");
    tc_iph_install(); tc_iph_count = 0;
    p_freea(NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    p = (unsigned char *)p_malloca(v_small);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)i;
    CHECK_EQ_I(p[63], 63);
    p_freea(p);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_malloca = (malloca_t)tc_crt_proc("_malloca"); p_freea = (freea_t)tc_crt_proc("_freea");
    TC_RUN(exported); TC_RUN(size_max); TC_RUN(freea_null_and_normal);
    return tc_end();
}
