/* TC
 * id:    hp_neg_new_handler_not_called_on_success
 * area:  heap
 * kind:  negative
 * funcs: malloc free _set_new_mode _set_new_handler _query_new_mode
 * crt:   win10
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _set_new_handler: handler is invoked only when allocation fails
 */
#include "tc.h"
#include <stdlib.h>


static int hcount;
static int __cdecl handler(size_t n) { (void)n; hcount++; return 0; }
static volatile size_t v_sz = 64;

TC_CASE(success_no_call)
{
    int old = _query_new_mode(); void *p;
    _set_new_mode(1); _set_new_handler(handler); hcount = 0;
    p = malloc(v_sz);
    CHECK(p != NULL);
    free(p);
    CHECK_EQ_I(hcount, 0);
    _set_new_handler(NULL); _set_new_mode(old);
}
int main(void) { tc_begin(); TC_RUN(success_no_call); return tc_end(); }
