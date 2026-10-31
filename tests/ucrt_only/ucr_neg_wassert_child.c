/* TC
 * id:    ucr_neg_wassert_child
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _wassert _set_error_mode _set_abort_behavior
 * crt:   win10 msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs assert/_wassert (failure prints diagnostic to stderr/message box and calls abort); abort exit code 3 when _CALL_REPORTFAULT clear
 * note:  Child sets _set_error_mode(_OUT_TO_STDERR=1) so no message box can appear and clears _CALL_REPORTFAULT (0x2) via
 *        _set_abort_behavior so the exit code is 3. msvcr80/msvcrt.dll are left out (set_abort_behavior export unsure there).
 */
#include "tc.h"

typedef void (__cdecl *wassert_t)(const wchar_t *, const wchar_t *, unsigned);
typedef int (__cdecl *sem_t)(int);
typedef unsigned (__cdecl *sab_t)(unsigned, unsigned);

TC_CASE(wassert) { DWORD c = 0; CHECK(tc_spawn_self("wassert", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sem_t s = (sem_t)tc_crt_proc("_set_error_mode"); sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        wassert_t f = (wassert_t)tc_crt_proc("_wassert");
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (s && sab && f) { s(1); sab(0, 2); f(L"x", L"f.c", 1); }
        return 77;
    }
    TC_RUN(wassert);
    return tc_end();
}
