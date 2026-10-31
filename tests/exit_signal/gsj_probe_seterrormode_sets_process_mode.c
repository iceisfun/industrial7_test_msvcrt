/* TC
 * id:    gsj_probe_seterrormode_sets_process_mode
 * area:  exit_signal
 * funcs:
 * procs: _seterrormode
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs SetErrorMode / GetErrorMode (Win7+)
 * note:  Probes that the legacy undocumented _seterrormode is a thin wrapper over SetErrorMode. Distinct from _set_error_mode.
 */
#include "tc.h"

typedef void (__cdecl *sem_t)(int);
typedef UINT (WINAPI *gem_t)(void);

TC_CASE(sets_and_clears)
{
    sem_t f = (sem_t)tc_crt_proc("_seterrormode");
    gem_t g = (gem_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetErrorMode");
    UINT orig;
    if (!f || !g) TC_SKIP("missing _seterrormode or GetErrorMode");
    orig = g();
    f(SEM_NOGPFAULTERRORBOX);
    CHECK((g() & SEM_NOGPFAULTERRORBOX) != 0);
    f(0);
    CHECK_EQ_U(g(), 0);
    SetErrorMode(orig);
}
int main(void) { tc_begin(); TC_RUN(sets_and_clears); return tc_end(); }
