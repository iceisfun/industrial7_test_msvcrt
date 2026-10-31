/* TC
 * id:    loc_configthreadlocale_modes
 * area:  locale
 * funcs: _configthreadlocale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _configthreadlocale
 * note:  _ENABLE_PER_THREAD_LOCALE=1, _DISABLE_PER_THREAD_LOCALE=2. Returns the previous setting. The initial state is probed in
 *        loc_probe_configthreadlocale_initial; this test only relies on values it set itself. Restores disabled mode at the end.
 */
#include "tc.h"
#include <locale.h>

static volatile int v_mode;
static int prev_before;

TC_CASE(enable_returns_previous)
{
    int q;
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    q = _configthreadlocale(0);
    CHECK_EQ_I(q, _DISABLE_PER_THREAD_LOCALE);
    v_mode = _ENABLE_PER_THREAD_LOCALE;
    prev_before = _configthreadlocale(v_mode);
    CHECK_EQ_I(prev_before, _DISABLE_PER_THREAD_LOCALE);
}
TC_CASE(query_after_enable_is_1) { CHECK_EQ_I(_configthreadlocale(0), _ENABLE_PER_THREAD_LOCALE); CHECK_EQ_I(_ENABLE_PER_THREAD_LOCALE, 1); }
TC_CASE(disable_returns_1)
{
    v_mode = _DISABLE_PER_THREAD_LOCALE;
    CHECK_EQ_I(_configthreadlocale(v_mode), _ENABLE_PER_THREAD_LOCALE);
}
TC_CASE(query_after_disable_is_2) { CHECK_EQ_I(_configthreadlocale(0), _DISABLE_PER_THREAD_LOCALE); CHECK_EQ_I(_DISABLE_PER_THREAD_LOCALE, 2); }
int main(void)
{
    tc_begin();
    TC_RUN(enable_returns_previous); TC_RUN(query_after_enable_is_1); TC_RUN(disable_returns_1); TC_RUN(query_after_disable_is_2);
    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);
    setlocale(LC_ALL, "C");
    return tc_end();
}
