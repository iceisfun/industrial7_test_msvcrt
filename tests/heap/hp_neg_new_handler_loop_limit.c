/* TC
 * id:    hp_neg_new_handler_loop_limit
 * area:  heap
 * kind:  negative
 * funcs: malloc _set_new_mode _set_new_handler _query_new_mode
 * crt:   win10
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs _set_new_mode, _set_new_handler, _callnewh (nonzero return from the handler = retry the allocation, 0 = give up)
 * note:  The exact retry count (4) and errno ENOMEM are reasoned from the retry contract, not quoted from docs.
 *        With new mode 1, malloc of an unsatisfiable size (below _HEAP_MAXREQ so HeapAlloc is actually attempted) calls the
 *        handler on each failure; a handler returning nonzero requests a retry, 0 stops with NULL and errno ENOMEM.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <new.h>

static volatile size_t v_huge = SIZE_MAX / 2;
static int hcount;
static int __cdecl handler(size_t n) { (void)n; hcount++; return hcount <= 3 ? 1 : 0; }

TC_CASE(retry_until_zero)
{
    int oldmode = _query_new_mode(); void *p;
    _set_new_mode(1); _set_new_handler(handler); hcount = 0; errno = 0;
    p = malloc(v_huge);
    _set_new_handler(NULL); _set_new_mode(oldmode);
    CHECK(p == NULL);
    if (p) free(p);
    CHECK_EQ_I(hcount, 4);
    CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(restored)
{
    void *p; hcount = 0; errno = 0;
    p = malloc(v_huge);
    CHECK(p == NULL);
    if (p) free(p);
    CHECK_EQ_I(hcount, 0);
    CHECK_EQ_I(errno, ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(retry_until_zero); TC_RUN(restored); return tc_end(); }
