/* TC
 * id:    exp_neg_ucrt_removed_sbh_probe
 * area:  exports
 * funcs:
 * procs: _set_sbh_threshold _get_sbh_threshold _set_amblksiz _get_amblksiz
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   inferred from CRT history: small-block-heap tuning APIs
 * note:  kind=negative. Probing: _set/_get_sbh_threshold and _set/_get_amblksiz were removed along with the SBH in UCRT.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(each_absent)
{
    static const char *const v[] = { "_set_sbh_threshold", "_get_sbh_threshold", "_set_amblksiz", "_get_amblksiz" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(each_absent);
    return tc_end();
}
