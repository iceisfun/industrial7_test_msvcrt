/* TC
 * id:    mth_neg_child_unmasked_invalid_fpe_probe
 * area:  math_basic
 * funcs:
 * procs: _controlfp_s sqrt
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s, _EM_INVALID (unmasked invalid-operation raises an exception)
 * note:  PROBE: a child that unmasks _EM_INVALID and calls sqrt(-1) does not return normally (exit code is an
 *        exception status such as 0xC0000090 or the CRT's own error exit), so it must not be 77 and must not be 0.
 *        Control: a child with default masks returns 77.
 */
#include "tc.h"
#include <math.h>
#include <float.h>

typedef int (__cdecl *cfs_t)(unsigned int *, unsigned int, unsigned int);
typedef double (__cdecl *sqrt_t)(double);
static volatile double v_m1 = -1.0, v_r;

TC_CASE(unmasked_child_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("unmasked", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(default_child_survives)
{
    DWORD code = 0; int ran = tc_spawn_self("masked", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'u') {
            cfs_t cfs = (cfs_t)tc_crt_proc("_controlfp_s");
            unsigned int cur;
            if (!cfs) return 78;
            SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
            cfs(&cur, 0, 0x00000010u);   /* _EM_INVALID == 0x10: clear its mask bit => unmasked */
        }
        { sqrt_t sq = (sqrt_t)tc_crt_proc("sqrt"); if (!sq) return 79; v_r = sq(v_m1); }
        return 77;
    }
    TC_RUN(unmasked_child_dies); TC_RUN(default_child_survives);
    return tc_end();
}
