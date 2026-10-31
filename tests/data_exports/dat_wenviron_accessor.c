/* TC
 * id:    dat_wenviron_accessor
 * area:  data_exports
 * funcs: _wputenv
 * procs: __p__wenviron
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _environ, _wenviron: _wenviron is NULL in a main() program until _wputenv/_wgetenv creates it
 * note:  Only asserts that _wputenv yields a non-NULL, NULL-terminated _wenviron containing the new entry.
 *        Narrow/wide cross-visibility is in dat_environ_wide_narrow_sync_probe.
 */
#include "tc.h"
#include <stdlib.h>

typedef wchar_t ***(__cdecl *pw_t)(void);
static pw_t p_wenviron;

TC_CASE(wputenv_creates_wenviron)
{
    CHECK(p_wenviron != NULL);
    if (!p_wenviron) return;
    CHECK(p_wenviron() != NULL);
    CHECK_EQ_I(_wputenv(L"TC_DWA_X=one"), 0);
    CHECK(*p_wenviron() != NULL);
}
TC_CASE(entry_present)
{
    wchar_t **e; int i, found = 0, n = 0;
    if (!p_wenviron) TC_SKIP("not exported");
    CHECK_EQ_I(_wputenv(L"TC_DWA_Y=two"), 0);
    e = *p_wenviron();
    CHECK(e != NULL);
    for (i = 0; e && e[i] && i < 100000; i++) { n++; if (tc_wcsequ(e[i], L"TC_DWA_Y=two")) found = 1; }
    CHECK(found);
    CHECK(e == NULL || e[n] == NULL);
    _wputenv(L"TC_DWA_X="); _wputenv(L"TC_DWA_Y=");
}

int main(void)
{
    tc_begin();
    p_wenviron = (pw_t)tc_crt_proc("__p__wenviron");
    TC_RUN(wputenv_creates_wenviron); TC_RUN(entry_present);
    return tc_end();
}
