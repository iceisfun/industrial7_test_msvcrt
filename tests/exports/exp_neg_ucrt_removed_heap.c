/* TC
 * id:    exp_neg_ucrt_removed_heap
 * area:  exports
 * funcs:
 * procs: _heapadd _heapset _heapused
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs UCRT breaking changes (heap functions)
 * note:  kind=negative. _heapchk/_heapmin/_heapwalk remain, see exp_heap_misc_exported.
 */
#include "tc.h"

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}

TC_CASE(control_present) { CHECK(crt != NULL); CHECK(crt && GetProcAddress(crt, "malloc") != NULL); }
TC_CASE(heapadd_absent)
{
    static const char *const v[] = { "_heapadd" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(heapset_absent)
{
    static const char *const v[] = { "_heapset" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}
TC_CASE(heapused_absent)
{
    static const char *const v[] = { "_heapused" };
    size_t i;
    for (i = 0; i < sizeof v / sizeof v[0]; i++) absent(v[i]);
}

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control_present);
    TC_RUN(heapadd_absent);
    TC_RUN(heapset_absent);
    TC_RUN(heapused_absent);
    return tc_end();
}
