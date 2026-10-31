/* TC
 * id:    hp_aligned_realloc_preserve
 * area:  heap
 * funcs: _aligned_realloc _aligned_malloc _aligned_free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_realloc
 * note:  NULL-memblock behaviour is probed separately in hp_probe_aligned_realloc_null.
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_a = 64;

static void fill(unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 7 + 3); }
static int check(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(i * 7 + 3)) return 0; return 1; }

TC_CASE(grow)
{
    unsigned char *p = (unsigned char *)_aligned_malloc(32, v_a), *q;
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 64, 0);
    fill(p, 32);
    q = (unsigned char *)_aligned_realloc(p, 4096, v_a);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 64, 0);
    CHECK(check(q, 32));
    fill(q, 4096); CHECK(check(q, 4096));
    _aligned_free(q);
}
TC_CASE(shrink)
{
    unsigned char *p = (unsigned char *)_aligned_malloc(4096, v_a), *q;
    CHECK(p != NULL); if (!p) return;
    fill(p, 4096);
    q = (unsigned char *)_aligned_realloc(p, 10, v_a);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 64, 0);
    CHECK(check(q, 10));
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(grow); TC_RUN(shrink); return tc_end(); }
