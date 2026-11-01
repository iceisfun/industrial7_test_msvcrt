/* TC
 * id:    ucr_crt_debugger_hook_probe
 * area:  ucrt_only
 * funcs:
 * procs: _crt_debugger_hook
 * crt:   win10
 * tier:  p2
 * conf:  observed
 * ref:   exported by ucrtbase (undocumented)
 * note:  Win10 22H2 x64 ucrtbase (observed via GetProcAddress): _crt_debugger_hook is NOT exported on x64; the x86 build is expected to export it
 *        and the call branch is kept for x86 [unverified on x86].
 */
#include "tc.h"

TC_CASE(callable)
{
    void (__cdecl *f)(int) = (void (__cdecl *)(int))tc_crt_proc("_crt_debugger_hook");
#ifdef _WIN64
    CHECK(f == NULL);   /* observed on Win10 22H2 x64: _crt_debugger_hook is not exported (x86 only) */
#else
    CHECK(f != NULL);
    if (!f) return;
    f(1);
#endif
}
int main(void) { tc_begin(); TC_RUN(callable); return tc_end(); }
