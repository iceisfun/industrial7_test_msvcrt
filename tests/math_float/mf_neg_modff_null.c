/* TC
 * id:    mf_neg_modff_null
 * area:  math_float
 * funcs:
 * procs: modff
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.6.12 (iptr must be valid; NULL is undefined)
 * note:  PROBE: what happens on real Windows when modff(1.5f, NULL) is called. Only asserts the child terminates (no hang);
 *        the exit code is recorded in the output line "PROBE exit=..." (likely 0xC0000005 access violation, or 0).
 */
#include "tc.h"
typedef float (__cdecl *modff_t)(float, float *);
static volatile float v_x = 1.5f;

static int child_main(const char *n)
{
    modff_t p = (modff_t)tc_crt_proc("modff");
    (void)n;
    if (!p) return 78;
    p(v_x, NULL);
    return 77;
}
TC_CASE(null_iptr_child)
{
    DWORD code = 0; char b[32];
    if (!tc_crt_proc("modff")) TC_SKIP("modff not exported");
    CHECK(tc_spawn_self("nullptr", &code, 20000));
    tc_hex(code, b); tc_write("PROBE exit="); tc_write(b); tc_write("\n");
    CHECK(code != 0xdead0002 && code != 0xdead0001);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(null_iptr_child);
    return tc_end();
}
