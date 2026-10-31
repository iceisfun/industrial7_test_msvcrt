/* TC
 * id:    gsj_probe_lock_locales_balanced
 * area:  exit_signal
 * funcs: setlocale
 * procs: _lock_locales _unlock_locales
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented ucrt internal exports)
 * note:  PROBING that _lock_locales/_unlock_locales are recursive on one thread, return, and do not block setlocale(NULL query)
 *        on the same thread. No threads, no unbalanced unlock.
 */
#include "tc.h"
#include <locale.h>

TC_DECL_PROC(void, _lock_locales, (void));
TC_DECL_PROC(void, _unlock_locales, (void));
static const char *volatile v_q;

TC_CASE(lock_unlock) { if (!tc_p__lock_locales || !tc_p__unlock_locales) TC_SKIP("missing"); tc_p__lock_locales(); tc_p__unlock_locales(); }
TC_CASE(recursive)
{
    if (!tc_p__lock_locales || !tc_p__unlock_locales) TC_SKIP("missing");
    tc_p__lock_locales(); tc_p__lock_locales(); tc_p__unlock_locales(); tc_p__unlock_locales();
}
TC_CASE(setlocale_while_locked)
{
    const char *s;
    if (!tc_p__lock_locales || !tc_p__unlock_locales) TC_SKIP("missing");
    tc_p__lock_locales();
    s = setlocale(LC_ALL, NULL);
    tc_p__unlock_locales();
    CHECK_STR(s, "C");
}
TC_CASE(many_iterations)
{
    int i;
    if (!tc_p__lock_locales || !tc_p__unlock_locales) TC_SKIP("missing");
    for (i = 0; i < 1000; i++) { tc_p__lock_locales(); tc_p__unlock_locales(); }
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_lock_locales); TC_LOAD_PROC(_unlock_locales);
    TC_RUN(lock_unlock); TC_RUN(recursive); TC_RUN(setlocale_while_locked); TC_RUN(many_iterations);
    return tc_end();
}
