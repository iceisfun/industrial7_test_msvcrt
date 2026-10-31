/* TC
 * id:    pfv_neg_probe_default_handler_fatal_win7crt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler
 * note:  PROBE: with no handler installed, sprintf(b, NULL) terminates the child; exit code expected 0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER) or 0xC0000409 (host/OS dependent). The real code is the finding.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile nullfmt = NULL;
TC_CASE(null_format_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[16]; sprintf(b, nullfmt); return 77; }
    TC_RUN(null_format_fatal); return tc_end();
}
