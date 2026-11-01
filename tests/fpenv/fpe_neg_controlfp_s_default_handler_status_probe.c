/* TC
 * id:    fpe_neg_controlfp_s_default_handler_status_probe
 * area:  fpenv
 * funcs: _controlfp_s
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation
 * note:  OBSERVED on Win10 22H2 (harness fix, class F/A): the exe's startup (mingw-w64) installs a non-fatal invalid-parameter handler, so _controlfp_s(&c, m, m) with an
 *        invalid bit returned EINVAL (22) and the child exited 77; the child now resets the handler to the UCRT default with _set_invalid_parameter_handler(NULL).
 *        Also the old call _controlfp_s(&c, 0, 0x4000) returned normally (child exit 77) (the validation evidently does not fire for newControl=0). Now newControl=mask=0x4000 (an undefined bit) -> EINVAL.
 *        Probes the exact termination status of the UCRT default handler path from _controlfp_s with an invalid mask: expected fast-fail 0xC0000409.
 */
#include "tc.h"
#include <float.h>

TC_CASE(status)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("badmask", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        unsigned c = 0; volatile unsigned m = 0x00004000;
        /* mingw-w64's exe startup installs its own (returning) invalid-parameter handler; restore the UCRT default (NULL) to observe it */
        void *(__cdecl *seth)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_set_invalid_parameter_handler");
        if (seth) seth(NULL);
        _controlfp_s(&c, m, m); return 77;
    }
    TC_RUN(status);
    return tc_end();
}
