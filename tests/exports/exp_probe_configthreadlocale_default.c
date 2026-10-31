/* TC
 * id:    exp_probe_configthreadlocale_default
 * area:  exports
 * funcs: 
 * procs: _configthreadlocale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _configthreadlocale
 * note:  Probing: in a fresh process _configthreadlocale(0) returns _DISABLE_PER_THREAD_LOCALE (2): per-thread locale is not enabled, the global locale is used.
 *        Docs say the function returns the previous setting, which is only ever _ENABLE_PER_THREAD_LOCALE (1) or _DISABLE_PER_THREAD_LOCALE (2) (or -1 on error), so 0 cannot be right.
 */
#include "tc.h"

typedef int (__cdecl *cfg_t)(int);

TC_CASE(default_is_disable)
{
    cfg_t p = (cfg_t)tc_crt_proc("_configthreadlocale");
    CHECK(p != NULL);
    if (!p) return;
    CHECK_EQ_I(p(0), 2 /* _DISABLE_PER_THREAD_LOCALE */);
}
int main(void) { tc_begin(); TC_RUN(default_is_disable); return tc_end(); }
