/* TC
 * id:    hp_cross_api_msize_realloc_expand
 * area:  heap
 * funcs: _msize _expand realloc malloc free
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _expand, _msize, realloc
 * note:  _expand may legitimately fail to grow in place; only consistency is asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n;

TC_CASE(expand_realloc_msize)
{
    unsigned char *p, *r; size_t i;
    v_n = 100; p = (unsigned char *)malloc(v_n);
    CHECK(p != NULL); if (!p) return;
    CHECK(_msize(p) >= 100);
    for (i = 0; i < 100; i++) p[i] = (unsigned char)i;
    v_n = 50; r = (unsigned char *)_expand(p, v_n);   /* shrink in place */
    if (r) { CHECK_EQ_P(r, p); CHECK(_msize(p) >= 50); }
    v_n = 400; r = (unsigned char *)_expand(p, v_n);  /* grow: success must return same pointer */
    if (r) { CHECK_EQ_P(r, p); CHECK(_msize(p) >= 400); }
    for (i = 0; i < 50; i++) if (p[i] != (unsigned char)i) { CHECK(0); break; }
    v_n = 1000; r = (unsigned char *)realloc(p, v_n);
    CHECK(r != NULL);
    if (!r) { free(p); return; }
    p = r;
    CHECK(_msize(p) >= 1000);
    for (i = 0; i < 50; i++) if (p[i] != (unsigned char)i) { CHECK(0); break; }
    free(p);
}
TC_CASE(msize_after_realloc_shrink)
{
    unsigned char *p; v_n = 500; p = (unsigned char *)malloc(v_n);
    CHECK(p != NULL); if (!p) return;
    v_n = 20; p = (unsigned char *)realloc(p, v_n);
    CHECK(p != NULL); if (!p) return;
    CHECK(_msize(p) >= 20);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(expand_realloc_msize); TC_RUN(msize_after_realloc_shrink); return tc_end(); }
