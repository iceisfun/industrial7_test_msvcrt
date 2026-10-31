/* TC
 * id:    exp_neg_c99_math_absent_msvcrt_probe
 * area:  exports
 * funcs:
 * procs: round trunc cbrt log2 expm1 tgamma fmax fma lround acosh
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   Win7 msvcrt.dll export table
 * note:  Probing: the Win7 system msvcrt.dll exports none of the C99 math functions.
 */
#include "tc.h"

static void chk(const char *const *n, int cnt)
{
    int i; for (i = 0; i < cnt; i++) CHECK(tc_crt_proc(n[i]) == NULL);
}
TC_CASE(each_absent) { static const char *const n[] = {"round", "trunc", "cbrt", "log2", "expm1", "tgamma", "fmax", "fma", "lround", "acosh"}; chk(n, 10); }

int main(void)
{
    tc_begin();
    TC_RUN(each_absent);
    return tc_end();
}
