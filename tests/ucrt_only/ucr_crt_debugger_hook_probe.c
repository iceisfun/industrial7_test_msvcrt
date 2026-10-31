/* TC
 * id:    ucr_crt_debugger_hook_probe
 * area:  ucrt_only
 * funcs:
 * procs: _crt_debugger_hook
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   exported by ucrtbase (undocumented)
 * note:  PROBE: _crt_debugger_hook(1) is exported and returns without crashing when no debugger is attached.
 */
#include "tc.h"

TC_CASE(callable)
{
    void (__cdecl *f)(int) = (void (__cdecl *)(int))tc_crt_proc("_crt_debugger_hook");
    CHECK(f != NULL);
    if (!f) return;
    f(1);
    CHECK(1);
}
int main(void) { tc_begin(); TC_RUN(callable); return tc_end(); }
