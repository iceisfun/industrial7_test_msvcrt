/* TC
 * id:    pfs_neg_fatal_default_handler_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s sprintf
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation: with no invalid-parameter handler the process is terminated
 * note:  Fatal paths run in a child (tc_spawn_self). The child returns 77 only if the CRT failed to terminate.
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile nullfmt = NULL;
static const char * volatile sfmt = "%s";

TC_CASE(overflow_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(null_format_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c; char b[2];
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();
        if (c[0] == 'o') sprintf_s(b, 2, sfmt, "abc");
        else sprintf(b, nullfmt);
        return 77;
    }
    TC_RUN(overflow_terminates); TC_RUN(null_format_terminates);
    return tc_end();
}
