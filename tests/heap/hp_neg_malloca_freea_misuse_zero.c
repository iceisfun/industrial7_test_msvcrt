/* TC
 * id:    hp_neg_malloca_freea_misuse_zero
 * area:  heap
 * kind:  negative
 * funcs: malloc free
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _malloca, _freea (_freea(NULL) is a no-op; sizes up to _ALLOCA_S_THRESHOLD use the stack, larger the heap)
 * note:  _malloca/_freea are inline header code (marker word before the block), so only malloc/free are CRT imports. Edge sizes:
 *        0, the stack/heap threshold boundary and one past it; none may fail, leak or trigger the invalid-parameter handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n;

TC_CASE(zero_size)
{
    void *p; v_n = 0;
    tc_iph_install(); tc_iph_count = 0;
    p = _malloca(v_n);
    CHECK(p != NULL);
    _freea(p);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(threshold_boundary)
{
    unsigned char *a, *b, *c, *d;
    tc_iph_install(); tc_iph_count = 0;
    v_n = _ALLOCA_S_THRESHOLD;      a = (unsigned char *)_malloca(v_n);
    v_n = _ALLOCA_S_THRESHOLD + 1;  b = (unsigned char *)_malloca(v_n);
    v_n = _ALLOCA_S_THRESHOLD - _ALLOCA_S_MARKER_SIZE;     c = (unsigned char *)_malloca(v_n);
    v_n = _ALLOCA_S_THRESHOLD - _ALLOCA_S_MARKER_SIZE + 1; d = (unsigned char *)_malloca(v_n);
    CHECK(a != NULL); CHECK(b != NULL); CHECK(c != NULL); CHECK(d != NULL);
    if (a) { a[0] = 1; a[_ALLOCA_S_THRESHOLD - 1] = 2; }
    if (b) { b[0] = 1; b[_ALLOCA_S_THRESHOLD] = 2; }
    if (c) { c[0] = 1; c[_ALLOCA_S_THRESHOLD - _ALLOCA_S_MARKER_SIZE - 1] = 2; }
    if (d) { d[0] = 1; d[_ALLOCA_S_THRESHOLD - _ALLOCA_S_MARKER_SIZE] = 2; }
    _freea(d); _freea(c); _freea(b); _freea(a);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(freea_null_twice)
{
    tc_iph_install(); tc_iph_count = 0;
    _freea(NULL); _freea(NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(zero_size); TC_RUN(threshold_boundary); TC_RUN(freea_null_twice); return tc_end(); }
