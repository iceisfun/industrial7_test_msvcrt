/* TC
 * id:    env_neg_getenv_s_default_fatal
 * area:  env
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s; Parameter validation
 * note:  Without a handler an invalid parameter terminates the process (child process).
 *        Win10 22H2 19045 (observed, ucrtbase): getenv_s(..., varname == NULL) is NOT validated: with the CRT default handler the child survives (r == 0, errno 0;
 *        Windows deviates from the MS docs table, class A). Case nullvar_default_survives asserts exactly that (exit 77). The validated row buffer == NULL with
 *        sizeInBytes > 0 (EINVAL) is the separate trigger "nullbuf": the default handler terminates the child (0xC0000409). The children restore the CRT default
 *        handler first (mingw startup installs its own, class F).
 *        [unverified] on older CRTs (msvcr90..120): there the original varname == NULL trigger and the abnormal-exit expectation are kept.
 */
#include "tc.h"
#include <stdlib.h>

#if TC_AT_LEAST(140)
#define TRIG "nullbuf"
#else
#define TRIG "nullvar"
#endif
TC_CASE(child_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self(TRIG, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);   /* 77 = child survived the call */
}
TC_CASE(parent_not_hung)
{
    DWORD code = 0; int ran = tc_spawn_self(TRIG, &code, 20000);
    CHECK(ran);                      /* child ended on its own, not by our timeout kill */
    CHECK(code != 0xdead0002u);
}
#if TC_AT_LEAST(140)
TC_CASE(nullvar_default_survives)
{
    DWORD code = 0; int ran = tc_spawn_self("nullvar", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);   /* observed: varname == NULL is not validated; child returned with r == 0 and survived */
}
#endif
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        size_t r = 99; char b[8];
        tc_iph_default();
        if (c[4] == 'b') { getenv_s(&r, NULL, sizeof b, "PATH"); return 77; }
#if TC_AT_LEAST(140)
        { errno_t e = getenv_s(&r, b, sizeof b, NULL); return (e == 0 && r == 0) ? 77 : 78; }
#else
        getenv_s(&r, b, sizeof b, NULL); return 77;
#endif
    }
    TC_RUN(child_abnormal_exit); TC_RUN(parent_not_hung);
#if TC_AT_LEAST(140)
    TC_RUN(nullvar_default_survives);
#endif
    return tc_end();
}
