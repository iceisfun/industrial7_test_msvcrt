/* TC
 * id:    nfx_neg_callnewh_handler_called_malloc_fail
 * area:  negative_fatal
 * funcs: _set_new_handler _set_new_mode malloc
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_new_mode ("malloc calls the new handler on failure when mode is 1"), _set_new_handler
 * note:  Size is SIZE_MAX/2, NOT SIZE_MAX: sizes above _HEAP_MAXREQ are rejected by malloc with ENOMEM before the
 *        new handler is ever consulted. The handler returns 0, which tells the CRT to stop retrying; malloc then returns NULL.
 */
#include "tc.h"
#include <stdlib.h>
typedef int (__cdecl *tc_pnh)(size_t);
int __cdecl _set_new_mode(int);
tc_pnh __cdecl _set_new_handler(tc_pnh);
#include <stdint.h>

static volatile int count;
static int __cdecl nh(size_t n) { (void)n; count++; return 0; }
static volatile size_t big = SIZE_MAX / 2;   /* <= _HEAP_MAXREQ (so the CRT really tries the heap + handler) but unallocatable */

TC_CASE(handler_called)
{
    tc_pnh oldh; int oldm;
    count = 0;
    oldm = _set_new_mode(1);
    oldh = _set_new_handler(nh);
    CHECK(malloc(big) == NULL);
    _set_new_handler(oldh);
    _set_new_mode(oldm);
    CHECK(count >= 1);
    CHECK_EQ_I(_set_new_mode(0), 0);
}
int main(void) { tc_begin(); TC_RUN(handler_called); return tc_end(); }
