/* TC
 * id:    gmb_get_fma3_enable_basic
 * area:  math
 * funcs:
 * procs: _get_FMA3_enable
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_FMA3_enable (x64 only)
 * note:  Skips when the exports are absent.
 */
#include "tc.h"

typedef int (__cdecl *get_t)(void);
static get_t p_get;

TC_CASE(exported) { CHECK(p_get != NULL); }
TC_CASE(zero_or_one)
{
    int a, b;
    if (!p_get) TC_SKIP("absent");
    a = p_get(); b = p_get();
    CHECK(a == 0 || a == 1);
    CHECK_EQ_I(a, b);
}

int main(void)
{
    tc_begin();
    p_get = (get_t)tc_crt_proc("_get_FMA3_enable");
    TC_RUN(exported); TC_RUN(zero_or_one);
    return tc_end();
}
