/* TC
 * id:    exp_string_conv_exported
 * area:  exports
 * funcs: atoi atol strtol strtoul _atoi64 _strtoi64 _strtoui64 _itoa _ltoa _ultoa _i64toa _ui64toa _gcvt _fcvt _ecvt
 * procs: atof strtod wcstol wcstoul wcstod _wtoi _wtol _wtoi64 _wcstoi64 _wcstoui64 _itow _ltow _ultow _i64tow _ui64tow
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs data conversion
 * note:  _wtof moved to exp_probe_wtof_exported because its presence in the oldest msvcrt.dll is not certain.
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

/* taking the address makes the linker import each name from the CRT under test */
static void *volatile tc_sink;
static void touch_imports(void)
{
    tc_sink = (void *)atoi;
    tc_sink = (void *)atol;
    tc_sink = (void *)strtol;
    tc_sink = (void *)strtoul;
    tc_sink = (void *)_atoi64;
    tc_sink = (void *)_strtoi64;
    tc_sink = (void *)_strtoui64;
    tc_sink = (void *)_itoa;
    tc_sink = (void *)_ltoa;
    tc_sink = (void *)_ultoa;
    tc_sink = (void *)_i64toa;
    tc_sink = (void *)_ui64toa;
    tc_sink = (void *)_gcvt;
    tc_sink = (void *)_fcvt;
    tc_sink = (void *)_ecvt;
}


TC_CASE(narrow_to_number)
{
    HAS("atoi");
    HAS("atol");
    HAS("atof");
    HAS("strtol");
    HAS("strtoul");
    HAS("strtod");
    HAS("_atoi64");
    HAS("_strtoi64");
    HAS("_strtoui64");
}

TC_CASE(narrow_from_number)
{
    HAS("_itoa");
    HAS("_ltoa");
    HAS("_ultoa");
    HAS("_i64toa");
    HAS("_ui64toa");
}

TC_CASE(wide_to_number)
{
    HAS("wcstol");
    HAS("wcstoul");
    HAS("wcstod");
    HAS("_wtoi");
    HAS("_wtol");
    HAS("_wtoi64");
    HAS("_wcstoi64");
    HAS("_wcstoui64");
}

TC_CASE(wide_from_number)
{
    HAS("_itow");
    HAS("_ltow");
    HAS("_ultow");
    HAS("_i64tow");
    HAS("_ui64tow");
}

TC_CASE(fp_formatting)
{
    HAS("_gcvt");
    HAS("_fcvt");
    HAS("_ecvt");
}

int main(void)
{
    tc_begin();
    touch_imports();
    TC_RUN(narrow_to_number);
    TC_RUN(narrow_from_number);
    TC_RUN(wide_to_number);
    TC_RUN(wide_from_number);
    TC_RUN(fp_formatting);
    return tc_end();
}
