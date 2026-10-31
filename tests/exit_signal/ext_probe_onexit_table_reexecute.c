/* TC
 * id:    ext_probe_onexit_table_reexecute
 * area:  exit_signal
 * funcs: _initialize_onexit_table _register_onexit_function _execute_onexit_table
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_startup.h onexit table API (undocumented)
 * note:  Probes that executing an already-executed table a second time returns 0 and runs no handler again.
 */
#include "tc.h"
#include <stdlib.h>
#include <corecrt_startup.h>

static int n;
static int __cdecl fa(void) { n++; return 0; }

TC_CASE(second_execute)
{
    _onexit_table_t t;
    t._first = t._last = t._end = (_PVFV *)(ULONG_PTR)0x1;
    CHECK_EQ_I(_initialize_onexit_table(&t), 0);
    CHECK_EQ_I(_register_onexit_function(&t, fa), 0);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(n, 1);
    CHECK_EQ_I(_execute_onexit_table(&t), 0);
    CHECK_EQ_I(n, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(second_execute);
    return tc_end();
}
