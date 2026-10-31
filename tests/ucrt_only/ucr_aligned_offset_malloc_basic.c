/* TC
 * id:    ucr_aligned_offset_malloc_basic
 * area:  ucrt_only
 * funcs: _aligned_offset_malloc _aligned_malloc _aligned_free
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc
 * note:  the address p+offset is aligned, not p itself
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t g_a, g_s, g_o;
static void *grab(size_t s, size_t a, size_t o) { g_s = s; g_a = a; g_o = o; return _aligned_offset_malloc(g_s, g_a, g_o); }
TC_CASE(a16_o4) { unsigned char *p = grab(100, 16, 4); CHECK(p != NULL); if (p) { CHECK_EQ_U(((uintptr_t)p + 4) % 16, 0); p[0] = 1; p[99] = 2; _aligned_free(p); } }
TC_CASE(a64_o24) { unsigned char *p = grab(200, 64, 24); CHECK(p != NULL); if (p) { CHECK_EQ_U(((uintptr_t)p + 24) % 64, 0); p[0] = 1; p[199] = 2; _aligned_free(p); } }
TC_CASE(offset0)
{
    unsigned char *p = grab(64, 32, 0), *q;
    CHECK(p != NULL); if (p) { CHECK_EQ_U((uintptr_t)p % 32, 0); _aligned_free(p); }
    g_s = 64; g_a = 32; q = _aligned_malloc(g_s, g_a);
    CHECK(q != NULL); if (q) { CHECK_EQ_U((uintptr_t)q % 32, 0); _aligned_free(q); }
}
int main(void) { tc_begin(); TC_RUN(a16_o4); TC_RUN(a64_o24); TC_RUN(offset0); return tc_end(); }
