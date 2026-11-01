/* TC
 * id:    pff_neg_percent_n_default_fatal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf %n; Parameter validation
 * note:  child process
 *        HARNESS FIX (class F, Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe5/6: every printf/scanf/strcpy_s "default handler" child exited 0x4d = survived): the mingw-w64 startup code installs
 *        a no-op invalid-parameter handler (call to _set_invalid_parameter_handler(__mingw_invalidParameterHandler) in main's crt startup), so the CRT's own
 *        default (terminate) never ran. The child now calls tc_iph_default() (= _set_invalid_parameter_handler(NULL)) first. Test expectations unchanged.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fatal_child)
{
    DWORD code = 0; int ran = tc_spawn_self("pn", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[64]; tc_iph_default(); volatile int n = 0; volatile double v = 1.5; sprintf(b, "%f%n", v, (int *)&n); return 77; }
    TC_RUN(fatal_child);
    return tc_end();
}
