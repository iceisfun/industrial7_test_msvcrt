/* TC
 * id:    hp_expand_grow_same_ptr
 * area:  heap
 * funcs: _expand malloc _msize free
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _expand
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(grow)
{
    unsigned char *p, *q; size_t i, bad = 0;
    v_n = 100; p = (unsigned char *)malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 100; i++) p[i] = (unsigned char)(i + 9);
    v_n = 200000; q = (unsigned char *)_expand(p, v_n);
    CHECK(q == NULL || q == p);
    for (i = 0; i < 100; i++) bad += p[i] != (unsigned char)(i + 9);
    CHECK_EQ_U(bad, 0);
    if (q) CHECK(_msize(q) >= 200000);
    else CHECK(_msize(p) >= 100);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(grow); return tc_end(); }
