/* TC
 * id:    prc_neg_beginthreadex_null_start_win7
 * area:  process
 * funcs: _beginthreadex _beginthread
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation": default invalid-parameter handler terminates the process
 * note:  PROBE: NULL start address is treated as an invalid parameter by the win7-era CRTs, so with the default handler the
 *        child terminates abnormally. Exact status not asserted (compare logs on real Windows); child returns 77 if it survives.
 */
#include "tc.h"
#include <process.h>

static unsigned __stdcall w(void *p) { (void)p; return 0; }
static void __cdecl w1(void *p) { (void)p; }
TC_CASE(ex_child_dies)
{
    DWORD code = 0; CHECK(tc_spawn_self("btnull", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(plain_child_dies)
{
    DWORD code = 0; CHECK(tc_spawn_self("btnull2", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[6] == '2') _beginthread(NULL, 0, NULL); else _beginthreadex(NULL, 0, NULL, NULL, 0, NULL);
        return 77;
    }
    (void)w; (void)w1;
    TC_RUN(ex_child_dies); TC_RUN(plain_child_dies);
    return tc_end();
}
