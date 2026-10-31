/* TC
 * id:    ucr_aligned_offset_realloc
 * area:  ucrt_only
 * funcs: _aligned_offset_malloc _aligned_offset_realloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_offset_realloc
 * note:  
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_s, g_a = 32, g_o = 8;
static void fill(unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 5 + 1); }
static int check(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(i * 5 + 1)) return 0; return 1; }
TC_CASE(grow)
{
    unsigned char *p, *q; g_s = 48; p = _aligned_offset_malloc(g_s, g_a, g_o); CHECK(p != NULL); if (!p) return;
    fill(p, 48); g_s = 2000; q = _aligned_offset_realloc(p, g_s, g_a, g_o);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U(((uintptr_t)q + 8) % 32, 0); CHECK(check(q, 48)); q[1999] = 1;
    _aligned_free(q);
}
TC_CASE(shrink)
{
    unsigned char *p, *q; g_s = 500; p = _aligned_offset_malloc(g_s, g_a, g_o); CHECK(p != NULL); if (!p) return;
    fill(p, 500); g_s = 24; q = _aligned_offset_realloc(p, g_s, g_a, g_o);
    CHECK(q != NULL); if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U(((uintptr_t)q + 8) % 32, 0); CHECK(check(q, 24));
    _aligned_free(q);
}
TC_CASE(null_block)
{
    unsigned char *q; g_s = 100; q = _aligned_offset_realloc(NULL, g_s, g_a, g_o);
    CHECK(q != NULL); if (q) { CHECK_EQ_U(((uintptr_t)q + 8) % 32, 0); q[99] = 1; _aligned_free(q); }
}
int main(void) { tc_begin(); TC_RUN(grow); TC_RUN(shrink); TC_RUN(null_block); return tc_end(); }
