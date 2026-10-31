/* TC
 * id:    dat_environ_data_win7
 * area:  data_exports
 * funcs: _putenv
 * procs: _environ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _environ (global data variable exported by the DLL CRTs)
 * note:  Resolves the _environ DATA export itself with GetProcAddress (the address of the variable), not the __p__environ accessor.
 *        ucrtbase is excluded: it exports only the __p__environ accessor and no _environ data symbol.
 */
#include "tc.h"
#include <stdlib.h>

static char ***pv;

static int find(const char *s)
{ char **e = *pv; int i; for (i = 0; e && e[i]; i++) if (tc_strequ(e[i], s)) return 1; return 0; }

TC_CASE(environ_nonnull_after_putenv)
{
    CHECK(pv != NULL);
    if (!pv) return;
    CHECK_EQ_I(_putenv("TC_DED_A=val"), 0);
    CHECK(*pv != NULL);
}
TC_CASE(entry_visible)
{
    if (!pv) TC_SKIP("_environ not exported");
    CHECK(find("TC_DED_A=val"));
    _putenv("TC_DED_A=");
}
TC_CASE(environ_null_terminated)
{
    char **e; int n = 0;
    if (!pv) TC_SKIP("_environ not exported");
    CHECK_EQ_I(_putenv("TC_DED_B=1"), 0);
    e = *pv;
    CHECK(e != NULL);
    while (e && e[n] && n < 100000) n++;
    CHECK(n < 100000);
    CHECK(e == NULL || e[n] == NULL);
    _putenv("TC_DED_B=");
}

int main(void)
{
    tc_begin();
    pv = (char ***)tc_crt_proc("_environ");
    TC_RUN(environ_nonnull_after_putenv); TC_RUN(entry_visible); TC_RUN(environ_null_terminated);
    return tc_end();
}
