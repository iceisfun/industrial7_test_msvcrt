/* TC
 * id:    dat_initenv_win7
 * area:  data_exports
 * funcs:
 * procs: __p___initenv __initenv
 * crt:   msvcr120 msvcr110 msvcr100 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs main (envp argument), _putenv ("may invalidate envp")
 * note:  Probe: __initenv holds the startup environment pointer; its VALUE (the pointer) is not changed by _putenv.
 *        The pointed-to block is never dereferenced after _putenv because docs say it may be invalidated.
 *        The data export __initenv is resolved at run time (address of the variable); skipped if not exported.
 */
#include "tc.h"
#include <stdlib.h>

typedef char ***(__cdecl *p_initenv_t)(void);
static char ***initenv_var;       /* address of the __initenv variable (data export) */
static p_initenv_t p_initenv_fn;

static char **getenv_ptr(void) { return p_initenv_fn ? *p_initenv_fn() : *initenv_var; }

TC_CASE(initenv_nonnull_or_null_consistent)
{
    char **env;
    if (!p_initenv_fn && !initenv_var) TC_SKIP("neither __p___initenv nor __initenv exported");
    env = getenv_ptr();
    if (initenv_var && p_initenv_fn) CHECK_EQ_P(*initenv_var, env);   /* data and accessor agree */
    if (env) {
        int i, n = 0;
        for (i = 0; env[i] && i < 100000; i++) {
            const char *s = env[i]; int has_eq = 0;
            while (*s) { if (*s == '=') has_eq = 1; s++; }
            if (has_eq) n++;
        }
        CHECK(n == i);   /* every entry is NAME=value (Windows hidden =C: style entries still contain '=') */
    }
}
TC_CASE(unchanged_by_putenv)
{
    char **before, **after;
    if (!p_initenv_fn && !initenv_var) TC_SKIP("neither __p___initenv nor __initenv exported");
    before = getenv_ptr();
    CHECK(_putenv("TC_DAT_INITENV_X=1") == 0);
    after = getenv_ptr();
    CHECK_EQ_P(after, before);
    if (initenv_var) CHECK_EQ_P(*initenv_var, before);
    CHECK_STR(getenv("TC_DAT_INITENV_X"), "1");
    _putenv("TC_DAT_INITENV_X=");
}
int main(void)
{
    tc_begin();
    initenv_var = (char ***)tc_crt_proc("__initenv");
    p_initenv_fn = (p_initenv_t)tc_crt_proc("__p___initenv");
    TC_RUN(initenv_nonnull_or_null_consistent); TC_RUN(unchanged_by_putenv);
    return tc_end();
}
