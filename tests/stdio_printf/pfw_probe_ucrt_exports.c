/* TC
 * id:    pfw_probe_ucrt_exports
 * area:  stdio_printf
 * funcs: 
 * procs: __stdio_common_vswprintf __stdio_common_vswprintf_s __stdio_common_vswprintf_p __stdio_common_vfwprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs: UCRT swprintf/wprintf/fwprintf are inline in headers over __stdio_common_*
 * note:  PROBE: ucrtbase exports __stdio_common_* wide helpers; and (separate case) does NOT export swprintf, wprintf, fwprintf. mingw libucrtbase.a has static thunks so the import lib cannot confirm this.
 */
#include "tc.h"

TC_CASE(stdio_common_present)
{
    CHECK(tc_crt_proc("__stdio_common_vswprintf") != NULL);
    CHECK(tc_crt_proc("__stdio_common_vswprintf_s") != NULL);
    CHECK(tc_crt_proc("__stdio_common_vswprintf_p") != NULL);
    CHECK(tc_crt_proc("__stdio_common_vfwprintf") != NULL);
}
TC_CASE(wprintf_swprintf_fwprintf_absent)
{
    CHECK(tc_crt_proc("wprintf") == NULL);
    CHECK(tc_crt_proc("swprintf") == NULL);
    CHECK(tc_crt_proc("fwprintf") == NULL);
}
int main(void) { tc_begin(); TC_RUN(stdio_common_present); TC_RUN(wprintf_swprintf_fwprintf_absent); return tc_end(); }
