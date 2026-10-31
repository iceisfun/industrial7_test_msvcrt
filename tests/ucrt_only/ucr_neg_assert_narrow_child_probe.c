/* TC
 * id:    ucr_neg_assert_narrow_child_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _assert _set_error_mode _set_abort_behavior
 * crt:   win10 msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _assert (legacy narrow form used by assert in older CRTs)
 * note:  Probes the narrow _assert export: on msvcr90-120 it should abort with exit code 3 (flags cleared); whether
 *        ucrtbase.dll exports _assert at all is unverified, so the case SKIPs when the export is absent.
 */
#include "tc.h"

typedef void (__cdecl *assert_t)(const char *, const char *, unsigned);
typedef int (__cdecl *sem_t)(int);
typedef unsigned (__cdecl *sab_t)(unsigned, unsigned);

TC_CASE(assert_narrow)
{
    DWORD c = 0;
    if (!tc_crt_proc("_assert")) TC_SKIP("_assert not exported");
    CHECK(tc_spawn_self("assert", &c, 20000)); CHECK_EQ_U(c, 3);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sem_t s = (sem_t)tc_crt_proc("_set_error_mode"); sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        assert_t f = (assert_t)tc_crt_proc("_assert");
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (s && sab && f) { s(1); sab(0, 2); f("x", "f.c", 1); }
        return 77;
    }
    TC_RUN(assert_narrow);
    return tc_end();
}
