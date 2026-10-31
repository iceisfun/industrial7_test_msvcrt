/* TC
 * id:    dat_commode_basic
 * area:  data_exports
 * funcs:
 * procs: __p__commode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _commode: global commit-mode flag, default _IOCOMMIT off (0) unless commode.obj is linked
 * note:  __p__commode is the DLL-CRT accessor for the _commode data variable.
 */
#include "tc.h"

typedef int *(__cdecl *pc_t)(void);
static pc_t p_commode;

TC_CASE(accessor_nonnull)
{
    CHECK(p_commode != NULL);
    if (p_commode) CHECK(p_commode() != NULL);
}
TC_CASE(default_is_zero_nocommit)
{
    if (!p_commode) TC_SKIP("not exported");
    CHECK_EQ_I(*p_commode(), 0);
}
TC_CASE(writable_roundtrip_restore)
{
    int old;
    if (!p_commode) TC_SKIP("not exported");
    old = *p_commode();
    *p_commode() = 0x4000;
    CHECK_EQ_I(*p_commode(), 0x4000);
    CHECK_EQ_P(p_commode(), p_commode());
    *p_commode() = old;
    CHECK_EQ_I(*p_commode(), old);
}

int main(void)
{
    tc_begin();
    p_commode = (pc_t)tc_crt_proc("__p__commode");
    TC_RUN(accessor_nonnull); TC_RUN(default_is_zero_nocommit); TC_RUN(writable_roundtrip_restore);
    return tc_end();
}
