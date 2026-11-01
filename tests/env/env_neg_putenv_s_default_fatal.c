/* TC
 * id:    env_neg_putenv_s_default_fatal
 * area:  env
 * kind:  negative
 * funcs:
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation": default handler terminates on _putenv_s(NULL, ...)
 * note:  Child exits abnormally (non-zero, not sentinel 77); exact status not asserted.
 */
#include "tc.h"

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));
static const char *volatile nullp = NULL;
static const char *volatile val = "v";

TC_CASE(child_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("putenv_s_null", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_putenv_s);
    if (tc_child_name()) { tc_iph_default(); if (tc_p__putenv_s) tc_p__putenv_s(nullp, val); return 77; }
    TC_RUN(child_abnormal_exit);
    return tc_end();
}
