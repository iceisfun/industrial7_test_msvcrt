/* TC
 * id:    gtm_neg_default_handler_fatal_strftime_internal
 * area:  time
 * funcs: free
 * procs: _Strftime _Gettnames
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strftime, 'Parameter validation'; UCRT default invalid-parameter handler terminates (fast fail)
 * note:  PROBE: internal _Strftime with a NULL tm and the default handler is assumed to hit the invalid-parameter path and
 *        terminate with 0xC0000409. Child exit 77 = returned, 78 = export missing.
 */
#include "tc.h"
#include <stdlib.h>

typedef void *(__cdecl *gett_t)(void);
typedef uintptr_t (__cdecl *sf_t)(char *, size_t, const char *, const void *, void *);

TC_CASE(null_tm_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("nulltm", &code, 20000);
    CHECK(ran);
    CHECK(code != 78);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(parent_alive) { CHECK(1); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        char b[64]; volatile const void *tm = NULL;
        gett_t g = (gett_t)tc_crt_proc("_Gettnames"); sf_t s = (sf_t)tc_crt_proc("_Strftime");
        if (!g || !s) return 78;
        s(b, sizeof b, "%Y", (const void *)tm, g());
        return 77;
    }
    TC_RUN(null_tm_fatal); TC_RUN(parent_alive);
    return tc_end();
}
