/* TC
 * id:    cpp_neg_purecall_default_exit3_probe_child
 * area:  cpp_runtime
 * funcs: _purecall
 * procs:
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _purecall, abort (exit code 3), _set_abort_behavior
 * note:  PROBE: default _purecall ends in abort(); with _CALL_REPORTFAULT cleared by tc_begin the exit code is 3.
 *        (msvcrt.dll uses _amsg_exit instead and is excluded.)
 */
#include "tc.h"

void __cdecl _purecall(void);

TC_CASE(abort_exit_code_3)
{
    DWORD code = 0; int ran = tc_spawn_self("pure", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _purecall(); return 77; }
    TC_RUN(abort_exit_code_3);
    return tc_end();
}
