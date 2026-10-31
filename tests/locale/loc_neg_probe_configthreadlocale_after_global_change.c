/* TC
 * id:    loc_neg_probe_configthreadlocale_after_global_change
 * area:  locale
 * funcs: _configthreadlocale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _configthreadlocale (returns previous setting; disabling after enabling)
 * note:  PROBE: after enabling per-thread locale, changing the thread's locale to 1252 and then disabling per-thread mode again,
 *        the state of the locale is unspecified; only a non-NULL query result is asserted. Re-enabling afterwards is expected to
 *        return the previous setting (2 = _DISABLE_PER_THREAD_LOCALE). Restores disabled mode and "C" at the end.
 */
#include "tc.h"
#include <locale.h>

static volatile int v_mode;

TC_CASE(disable_after_thread_locale_change)
{
    const char *r; char b[8];
    v_mode = _DISABLE_PER_THREAD_LOCALE; _configthreadlocale(v_mode);
    v_mode = _ENABLE_PER_THREAD_LOCALE; _configthreadlocale(v_mode);
    r = setlocale(LC_ALL, ".1252");
    if (!r) { _configthreadlocale(_DISABLE_PER_THREAD_LOCALE); TC_SKIP("1252 unavailable"); }
    v_mode = _DISABLE_PER_THREAD_LOCALE; _configthreadlocale(v_mode);
    r = setlocale(LC_ALL, NULL);
    CHECK(r != NULL);
    tc_write("TC INFO LC_ALL after disable = "); tc_write(r ? r : "(null)"); tc_write("\n");
    (void)b;
}
TC_CASE(reenable_returns_previous_disable)
{
    v_mode = _ENABLE_PER_THREAD_LOCALE;
    CHECK_EQ_I(_configthreadlocale(v_mode), _DISABLE_PER_THREAD_LOCALE);
    CHECK_EQ_I(_configthreadlocale(0), _ENABLE_PER_THREAD_LOCALE);
}
int main(void)
{
    tc_begin();
    TC_RUN(disable_after_thread_locale_change); TC_RUN(reenable_returns_previous_disable);
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    setlocale(LC_ALL, "C");
    return tc_end();
}
