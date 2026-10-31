/* TC
 * id:    ext_onexit_table_inprocess
 * area:  exit_signal
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   MS docs / corecrt_startup.h: _onexit_table_t {_first,_last,_end}
 * note:  _onexit_table_t is {_first,_last,_end}. Internal startup API (not formally documented); LIFO order follows atexit semantics.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static char order[8]; static int n;
static int __cdecl fa(void) { order[n++] = 'A'; return 0; }
static int __cdecl fb(void) { order[n++] = 'B'; return 0; }
static int __cdecl fc(void) { order[n++] = 'C'; return 0; }

TC_CASE(lifo)
{
    _onexit_table_t t; int r;
    t._first = t._last = t._end = (_PVFV *)(ULONG_PTR)0x1; /* equal pointers: table reads as uninitialised */
    r = _initialize_onexit_table(&t);
    CHECK_EQ_I(r, 0);
    CHECK(t._first == t._last);
    CHECK_EQ_I(_register_onexit_function(&t, fa), 0);
    CHECK_EQ_I(_register_onexit_function(&t, fb), 0);
    CHECK_EQ_I(_register_onexit_function(&t, fc), 0);
    CHECK_EQ_I(n, 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(n, 3);
    CHECK_STR(order, "CBA");
}
int main(void)
{
    tc_begin();
    TC_RUN(lifo);
    return tc_end();
}
