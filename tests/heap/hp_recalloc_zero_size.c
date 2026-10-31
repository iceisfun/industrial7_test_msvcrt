/* TC
 * id:    hp_recalloc_zero_size
 * area:  heap
 * funcs: _recalloc malloc
 * procs: 
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _recalloc, realloc (by analogy)
 * note:  PROBING that _recalloc(p,n,0) behaves like realloc(p,0): frees the block and returns NULL. MS docs for _recalloc do not state
 *        the zero-size case explicitly, so this is inferred from realloc.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n, v_s;

TC_CASE(zero)
{
    void *p;
    v_n = 8; p = malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    v_n = 1; v_s = 0;
    CHECK(_recalloc(p, v_n, v_s) == NULL);
}
int main(void) { tc_begin(); TC_RUN(zero); return tc_end(); }
