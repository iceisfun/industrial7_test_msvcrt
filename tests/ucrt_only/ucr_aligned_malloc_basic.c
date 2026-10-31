/* TC
 * id:    ucr_aligned_malloc_basic
 * area:  ucrt_only
 * funcs: _aligned_malloc _aligned_free
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_malloc, _aligned_free
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_sz = 100;

TC_CASE(alignments)
{
    size_t a;
    for (a = 1; a <= 4096; a <<= 1) {
        void *p = _aligned_malloc(v_sz, a);
        CHECK(p != NULL);
        if (p) { CHECK_EQ_U((size_t)(ULONG_PTR)p % a, 0); _aligned_free(p); }
    }
}
TC_CASE(writable)
{
    size_t i, a; 
    for (a = 16; a <= 4096; a <<= 3) {
        unsigned char *p = (unsigned char *)_aligned_malloc(5000, a);
        CHECK(p != NULL);
        if (!p) continue;
        for (i = 0; i < 5000; i++) p[i] = (unsigned char)i;
        for (i = 0; i < 5000; i++) if (p[i] != (unsigned char)i) { CHECK(0); break; }
        _aligned_free(p);
    }
}
TC_CASE(free_null) { _aligned_free(NULL); CHECK(1); }
TC_CASE(no_overlap)
{
    unsigned char *a = (unsigned char *)_aligned_malloc(256, 64), *b = (unsigned char *)_aligned_malloc(256, 64);
    CHECK(a && b);
    if (a && b) { CHECK(a + 256 <= b || b + 256 <= a); }
    _aligned_free(a); _aligned_free(b);
}

int main(void) { tc_begin(); TC_RUN(alignments); TC_RUN(writable); TC_RUN(free_null); TC_RUN(no_overlap); return tc_end(); }
