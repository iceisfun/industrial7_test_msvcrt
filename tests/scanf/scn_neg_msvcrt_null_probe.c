/* TC
 * id:    scn_neg_msvcrt_null_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   msvcrt.dll predates the CRT 8.0 parameter validation; NULL handling undocumented
 * note:  PROBES: sscanf(NULL,"%d",&x) / swscanf(NULL,...) in msvcrt.dll either return EOF (child exits 77) or crash with an
 *        access violation (0xC0000005). Anything else fails. Observed code is the measurement.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vs; static const wchar_t *volatile vw;
static void check_child(const char *n)
{
    DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK(code == 77 || code == TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(sscanf_null) { check_child("ss"); }
TC_CASE(swscanf_null) { check_child("sw"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int x = 0, r; vs = NULL; vw = NULL;
        r = tc_strequ(c, "ss") ? sscanf(vs, "%d", &x) : swscanf(vw, L"%d", &x);
        return r == EOF ? 77 : 78;
    }
    TC_RUN(sscanf_null); TC_RUN(swscanf_null);
    return tc_end();
}
