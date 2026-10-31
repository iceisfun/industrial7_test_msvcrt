/* TC
 * id:    gsj_probe_local_unwind_export
 * area:  exit_signal
 * funcs:
 * procs: _local_unwind
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   legacy x64 CRT export (SEH helper)
 * note:  Probes that _local_unwind is exported by the x64 win7-era CRTs and lives inside the CRT module. Not called.
 */
#include "tc.h"

TC_CASE(exported) { CHECK(tc_crt_proc("_local_unwind") != NULL); }
TC_CASE(inside_module)
{
    FARPROC p = tc_crt_proc("_local_unwind"); HMODULE m = NULL;
    if (!p) TC_SKIP("not exported");
    CHECK(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)p, &m));
    CHECK(m == GetModuleHandleA(TC_CRT_DLL));
}
int main(void) { tc_begin(); TC_RUN(exported); TC_RUN(inside_module); return tc_end(); }
