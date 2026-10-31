/* TC
 * id:    hp_neg_b2_realloc_huge
 * area:  heap
 * funcs: realloc malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs realloc (NULL and errno ENOMEM on failure; original block is not freed); malloc
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile size_t v_n;

TC_CASE(realloc_huge_keeps_original)
{
    unsigned char *p, *q; size_t i, bad = 0;
    v_n = 64; p = (unsigned char *)malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(i + 1);
    v_n = (size_t)-1; errno = 0;
    q = (unsigned char *)realloc(p, v_n);
    CHECK(q == NULL);
    if (q) { free(q); return; }
    CHECK_EQ_I(errno, ENOMEM);
    for (i = 0; i < 64; i++) bad += p[i] != (unsigned char)(i + 1);
    CHECK_EQ_U(bad, 0);
    free(p);
}
TC_CASE(malloc_huge)
{
    void *p; v_n = (size_t)-1; errno = 0;
    p = malloc(v_n);
    CHECK(p == NULL);
    if (p) free(p);
    else CHECK_EQ_I(errno, ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(realloc_huge_keeps_original); TC_RUN(malloc_huge); return tc_end(); }
