/* TC
 * id:    scn_neg_child_null_format_fastfail_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation" / _set_invalid_parameter_handler: without a handler the invalid-parameter path terminates the process; see err_neg_default_handler_fatal
 * note:  PROBE: exact termination status of sscanf("1", NULL, ...) with the default handler is 0xC0000409 (__fastfail) on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile nf;

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) { int x = 0; nf = NULL; (void)sscanf("1", nf, &x); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
