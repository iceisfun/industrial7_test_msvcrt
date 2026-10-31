/* TC
 * id:    exp_neg_ucrt_iob_func_absent_probe
 * area:  exports
 * funcs: 
 * procs: __iob_func
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   UCRT stdio.h: stdin == __acrt_iob_func(0)
 * note:  PROBE: ucrtbase.dll does not export __iob_func (it is only a legacy_stdio_definitions.lib shim over __acrt_iob_func). Reasoned, not documented.
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g_iob_func[] = { "__iob_func", NULL };

TC_CASE(iob_func) { absent(g_iob_func); }

int main(void)
{
    tc_begin();
    TC_RUN(iob_func);
    return tc_end();
}
