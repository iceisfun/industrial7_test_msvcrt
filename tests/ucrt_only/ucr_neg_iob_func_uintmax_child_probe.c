/* TC
 * id:    ucr_neg_iob_func_uintmax_child_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: __acrt_iob_func
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   UCRT implementation detail
 * note:  PROBES that __acrt_iob_func(UINT_MAX) merely returns (pointer arithmetic, no table lookup that faults).
 *        Child exit 0 = returned without crashing (the pointer is not dereferenced); a crash shows its NT status.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned vmax = 0xFFFFFFFFu;

TC_CASE(uintmax_child)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("uintmax", &code, 20000));
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        volatile void *p = __acrt_iob_func(vmax);
        (void)p;
        return 0;
    }
    TC_RUN(uintmax_child);
    return tc_end();
}
