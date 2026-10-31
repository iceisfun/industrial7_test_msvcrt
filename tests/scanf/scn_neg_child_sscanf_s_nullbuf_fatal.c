/* TC
 * id:    scn_neg_child_sscanf_s_nullbuf_fatal
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs scanf_s, sscanf_s: NULL destination pointer is an invalid parameter; default handler terminates
 * note:  PROBES: NULL destination for %s in sscanf_s is treated as an invalid parameter (fast-fail). Docs do not
 *        state this explicitly; the alternative is a plain return of 0/EOF (child exits 77) or an access violation.
 *        Child process, no handler installed. 77 means the call returned.
 */
#include "tc.h"
#include <stdio.h>

static char *volatile vbuf;
TC_CASE(child_null_buffer)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { vbuf = NULL; sscanf_s("abc", "%s", vbuf, (unsigned)8); return 77; }
    TC_RUN(child_null_buffer);
    return tc_end();
}
