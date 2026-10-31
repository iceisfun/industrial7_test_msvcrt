/* TC
 * id:    gmb_set_fma3_enable_probe
 * area:  math
 * funcs:
 * procs: _get_FMA3_enable _set_FMA3_enable
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_FMA3_enable (x64 only)
 * note:  PROBE: _set_FMA3_enable(0) returns 0 or 1 and afterwards _get_FMA3_enable() == 0; restoring the original value
 *        leaves the getter at 0 or 1. The exact return value of the setter is not asserted.
 */
#include "tc.h"

typedef int (__cdecl *get_t)(void);
typedef int (__cdecl *set_t)(int);
static get_t p_get; static set_t p_set;

TC_CASE(exported) { CHECK(p_get != NULL); CHECK(p_set != NULL); }
TC_CASE(set_zero_then_get)
{
    int orig, r, f;
    if (!p_get || !p_set) TC_SKIP("absent");
    orig = p_get();
    r = p_set(0);
    CHECK(r == 0 || r == 1);
    CHECK_EQ_I(p_get(), 0);
    p_set(orig);
    f = p_get();
    CHECK(f == 0 || f == 1);
}

int main(void)
{
    tc_begin();
    p_get = (get_t)tc_crt_proc("_get_FMA3_enable");
    p_set = (set_t)tc_crt_proc("_set_FMA3_enable");
    TC_RUN(exported); TC_RUN(set_zero_then_get);
    return tc_end();
}
