/* TC
 * id:    hp_realloc_same_size
 * area:  heap
 * funcs: realloc malloc free
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.5
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

static void one(size_t n)
{
    unsigned char *p, *q; size_t i, bad = 0;
    v_n = n; p = (unsigned char *)malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < n; i++) p[i] = (unsigned char)(i * 7 + 3);
    v_n = n; q = (unsigned char *)realloc(p, v_n);
    CHECK(q != NULL);
    if (!q) { free(p); return; }
    for (i = 0; i < n; i++) if (q[i] != (unsigned char)(i * 7 + 3)) bad++;
    CHECK_EQ_U(bad, 0);
    free(q);
}
TC_CASE(n1) { one(1); }
TC_CASE(n16) { one(16); }
TC_CASE(n4096) { one(4096); }
int main(void) { tc_begin(); TC_RUN(n1); TC_RUN(n16); TC_RUN(n4096); return tc_end(); }
