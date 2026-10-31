/* TC
 * id:    exp_neg_probe_ucrt_only_names_in_legacy
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: malloc
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Universal CRT (these entry points are part of the UCRT redesign)
 * note:  Probing: names introduced with the Universal CRT in VS2015 are not exported by the VS2005-VS2013 CRTs or msvcrt.dll.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)


TC_CASE(stdio_common_absent)
{
    HASNOT("__stdio_common_vfprintf");
    HASNOT("__stdio_common_vsprintf");
    HASNOT("__stdio_common_vfscanf");
}

TC_CASE(crt_atexit_absent)
{
    HASNOT("_crt_atexit");
}

TC_CASE(startup_api_absent)
{
    HASNOT("_configure_narrow_argv");
    HASNOT("_initialize_narrow_environment");
    HASNOT("_execute_onexit_table");
    HASNOT("_register_onexit_function");
}

int main(void)
{
    tc_begin();
    TC_RUN(stdio_common_absent);
    TC_RUN(crt_atexit_absent);
    TC_RUN(startup_api_absent);
    return tc_end();
}
