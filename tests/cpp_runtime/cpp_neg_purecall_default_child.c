/* TC
 * id:    cpp_neg_purecall_default_child
 * area:  cpp_runtime
 * funcs: _purecall
 * procs:
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _purecall / Runtime Error R6025 pure virtual function call
 * note:  With no handler _purecall reports R6025 and terminates the process (abort or _amsg_exit depending on version). Only
 *        'never returns, nonzero exit' is asserted here; the exact code is in the _exit3 probe test.
 */
#include "tc.h"

void __cdecl _purecall(void);

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("pure", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK(code != 0xdead0002u);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _purecall(); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
