/* TC
 * id:    hp_realloc_null_is_malloc
 * area:  heap
 * funcs: realloc free _msize
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.5; MS docs realloc
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(n64)
{
    unsigned char *p; size_t i;
    v_n = 64; p = (unsigned char *)realloc(NULL, v_n);
    CHECK(p != NULL);
    if (!p) return;
    CHECK(_msize(p) >= 64);
    for (i = 0; i < 64; i++) p[i] = (unsigned char)i;
    for (i = 0; i < 64; i++) if (p[i] != (unsigned char)i) { CHECK(0); break; }
    free(p);
}
TC_CASE(n1)
{
    unsigned char *p;
    v_n = 1; p = (unsigned char *)realloc(NULL, v_n);
    CHECK(p != NULL);
    if (!p) return;
    CHECK(_msize(p) >= 1);
    p[0] = 0x5a; CHECK_EQ_I(p[0], 0x5a);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(n64); TC_RUN(n1); return tc_end(); }
