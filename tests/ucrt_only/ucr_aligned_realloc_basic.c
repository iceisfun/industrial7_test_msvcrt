/* TC
 * id:    ucr_aligned_realloc_basic
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_realloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_realloc
 * note:  
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a = 32, g_s;
static void fill(unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 7 + 3); }
static int check(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(i * 7 + 3)) return 0; return 1; }
TC_CASE(grow)
{
    unsigned char *p, *q; g_s = 16; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    fill(p, 16); g_s = 4096; q = _aligned_realloc(p, g_s, g_a);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 32, 0); CHECK(check(q, 16)); q[4095] = 1;
    _aligned_free(q);
}
TC_CASE(shrink)
{
    unsigned char *p, *q; g_s = 1024; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    fill(p, 1024); g_s = 40; q = _aligned_realloc(p, g_s, g_a);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 32, 0); CHECK(check(q, 40));
    _aligned_free(q);
}
TC_CASE(null_block)
{
    unsigned char *q; g_s = 100; q = _aligned_realloc(NULL, g_s, g_a);
    CHECK(q != NULL); if (q) { CHECK_EQ_U((uintptr_t)q % 32, 0); q[99] = 1; _aligned_free(q); }
}
TC_CASE(same_size)
{
    unsigned char *p, *q; g_s = 64; p = _aligned_malloc(g_s, g_a); CHECK(p != NULL); if (!p) return;
    fill(p, 64); q = _aligned_realloc(p, g_s, g_a);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 32, 0); CHECK(check(q, 64));
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(grow); TC_RUN(shrink); TC_RUN(null_block); TC_RUN(same_size); return tc_end(); }
