/* TC
 * id:    hp_realloc_zero_frees
 * area:  heap
 * funcs: realloc malloc
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs realloc
 * note:  MS docs: if size is zero the block is freed and NULL is returned (differs from C, where the result is implementation-defined). p is not touched afterwards.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(zero_size)
{
    void *p, *q;
    v_n = 32; p = malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    v_n = 0; q = realloc(p, v_n);
    CHECK(q == NULL);
}
int main(void) { tc_begin(); TC_RUN(zero_size); return tc_end(); }
