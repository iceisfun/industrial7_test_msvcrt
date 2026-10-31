/* TC
 * id:    hp_aligned_mixed_with_malloc
 * area:  heap
 * funcs: _aligned_malloc _aligned_free malloc free
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_malloc, _aligned_free
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define N 24
static volatile size_t v_sz, v_al;

TC_CASE(interleave)
{
    void *p[N]; size_t sz[N]; int isal[N], i, j, bad = 0, nul = 0;
    static const size_t als[] = { 16, 32, 64, 128, 4096 };
    for (i = 0; i < N; i++) {
        v_sz = (size_t)(i * 37 + 8); sz[i] = v_sz; isal[i] = i & 1;
        if (isal[i]) { v_al = als[(i / 2) % 5]; p[i] = _aligned_malloc(v_sz, v_al); if (p[i] && ((size_t)p[i] % v_al)) bad++; }
        else p[i] = malloc(v_sz);
        if (!p[i]) { nul++; continue; }
        memset(p[i], i + 1, sz[i]);
    }
    CHECK_EQ_I(nul, 0);
    CHECK_EQ_I(bad, 0);
    for (i = 0; i < N; i++) {
        unsigned char *b = (unsigned char *)p[i]; size_t k;
        if (!b) continue;
        for (k = 0; k < sz[i]; k++) if (b[k] != (unsigned char)(i + 1)) { CHECK(0); break; }
        for (j = i + 1; j < N; j++) {
            unsigned char *c = (unsigned char *)p[j];
            if (c && b < c + sz[j] && c < b + sz[i]) { CHECK(0); }
        }
    }
    for (i = N - 1; i >= 0; i--) { if (!p[i]) continue; if (isal[i]) _aligned_free(p[i]); else free(p[i]); }
}
int main(void) { tc_begin(); TC_RUN(interleave); return tc_end(); }
