/* TC
 * id:    hp_neg_new_mode_off_malloc_no_handler
 * area:  heap
 * kind:  negative
 * funcs: malloc _set_new_mode _set_new_handler _query_new_mode
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_new_mode: mode 0 (default) means malloc does not call the new handler on failure; mode 1 means it does
 * note:  Uses SIZE_MAX-0x10000: unsatisfiable, yet below _HEAP_MAXREQ (0xFFFF...E0 on x64), so a size pre-check does not reject it and the new-handler path is reachable.
 */
#include "tc.h"
#include <stdlib.h>

#include <errno.h>

static int hcount;
static int __cdecl handler(size_t n) { (void)n; hcount++; return 0; }
static volatile size_t v_big = (size_t)-1 - 0x10000;

TC_CASE(mode0_no_call)
{
    int old = _query_new_mode(); void *p;
    _set_new_mode(0);
    _set_new_handler(handler); hcount = 0;
    p = malloc(v_big);
    CHECK(p == NULL);
    if (p) free(p);
    CHECK_EQ_I(hcount, 0);
    _set_new_handler(NULL); _set_new_mode(old);
}
TC_CASE(mode1_calls)
{
    int old = _query_new_mode(); void *p;
    _set_new_mode(1);
    _set_new_handler(handler); hcount = 0;
    p = malloc(v_big);
    CHECK(p == NULL);
    if (p) free(p);
    CHECK(hcount >= 1);
    _set_new_handler(NULL); _set_new_mode(old);
}
int main(void) { tc_begin(); TC_RUN(mode0_no_call); TC_RUN(mode1_calls); return tc_end(); }
