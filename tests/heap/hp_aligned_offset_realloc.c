/* TC
 * id:    hp_aligned_offset_realloc
 * area:  heap
 * funcs: _aligned_offset_realloc _aligned_offset_malloc _aligned_free
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _aligned_offset_realloc
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_a = 64, v_off = 8;

static void fill(unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 11 + 5); }
static int check(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(i * 11 + 5)) return 0; return 1; }

TC_CASE(grow)
{
    unsigned char *p = (unsigned char *)_aligned_offset_malloc(40, v_a, v_off), *q;
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U(((uintptr_t)p + 8) % 64, 0);
    fill(p, 40);
    q = (unsigned char *)_aligned_offset_realloc(p, 4000, v_a, v_off);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U(((uintptr_t)q + 8) % 64, 0);
    CHECK(check(q, 40));
    _aligned_free(q);
}
TC_CASE(shrink)
{
    unsigned char *p = (unsigned char *)_aligned_offset_malloc(4000, v_a, v_off), *q;
    CHECK(p != NULL); if (!p) return;
    fill(p, 4000);
    q = (unsigned char *)_aligned_offset_realloc(p, 20, v_a, v_off);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U(((uintptr_t)q + 8) % 64, 0);
    CHECK(check(q, 20));
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(grow); TC_RUN(shrink); return tc_end(); }
