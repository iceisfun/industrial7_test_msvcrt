/* TC
 * id:    ucr_set_app_type_probe
 * area:  ucrt_only
 * funcs:
 * procs: _set_app_type
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   corecrt_startup.h _crt_app_type (_crt_console_app = 1)
 * note:  PROBE: _set_app_type(1) is callable without crashing and string formatting through the CRT still works afterwards.
 */
#include "tc.h"
#include <stdio.h>

typedef void (__cdecl *sat_t)(int);
TC_CASE(call)
{
    sat_t f = (sat_t)tc_crt_proc("_set_app_type");
    char b[16];
    if (!f) TC_SKIP("not exported");
    f(1);
    sprintf(b, "ok%d", 5);
    CHECK_STR(b, "ok5");
}
int main(void) { tc_begin(); TC_RUN(call); return tc_end(); }
