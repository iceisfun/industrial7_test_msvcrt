/* TC
 * id:    nfx_neg_purecall_default_terminates
 * area:  negative_fatal
 * kind:  negative
 * funcs: _purecall
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs C Run-Time Error R6025 pure virtual function call
 * note:  Only abnormal termination is asserted, not the exact status (see nfx_neg_purecall_default_exact_probe).
 */
#include "tc.h"

extern void __cdecl _purecall(void);
TC_CASE(terminates)
{
    DWORD c = 0; CHECK(tc_spawn_self("pc", &c, 20000)); CHECK(c != 0); CHECK(c != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _purecall(); return 77; }
    TC_RUN(terminates); return tc_end();
}
