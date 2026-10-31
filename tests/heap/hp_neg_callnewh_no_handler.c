/* TC
 * id:    hp_neg_callnewh_no_handler
 * area:  heap
 * kind:  negative
 * funcs: _callnewh _set_new_handler
 * crt:   win10
 * tier:  p2
 * conf:  inferred
 * ref:   _callnewh is exported but undocumented; MS docs _set_new_handler describe the handler contract (0 = stop retrying). With no handler installed, or one that returns 0, no retry is requested (returns 0)
 */
#include "tc.h"
#include <stdlib.h>


int __cdecl _callnewh(size_t);
static int hcount, hret;
static int __cdecl handler(size_t n) { (void)n; hcount++; return hret; }
static volatile size_t v_sz = 8;

TC_CASE(no_handler)
{
    _set_new_handler(NULL);
    CHECK_EQ_I(_callnewh(v_sz), 0);
}
TC_CASE(handler_returns_zero)
{
    _set_new_handler(handler); hcount = 0; hret = 0;
    CHECK_EQ_I(_callnewh(v_sz), 0);
    CHECK_EQ_I(hcount, 1);
    _set_new_handler(NULL);
}
TC_CASE(handler_returns_nonzero)
{
    _set_new_handler(handler); hcount = 0; hret = 1;
    CHECK(_callnewh(v_sz) != 0);
    CHECK_EQ_I(hcount, 1);
    _set_new_handler(NULL);
}
int main(void) { tc_begin(); TC_RUN(no_handler); TC_RUN(handler_returns_zero); TC_RUN(handler_returns_nonzero); return tc_end(); }
