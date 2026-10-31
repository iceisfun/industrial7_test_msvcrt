/* TC
 * id:    hp_probe_realloc_null_zero
 * area:  heap
 * funcs: realloc free
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs realloc, malloc
 * note:  PROBING whether realloc(NULL,0) returns a valid non-NULL zero-size block like malloc(0).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(null_zero)
{
    void *p;
    v_n = 0; p = realloc(NULL, v_n);
    CHECK(p != NULL);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(null_zero); return tc_end(); }
