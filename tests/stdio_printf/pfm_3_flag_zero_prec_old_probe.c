/* TC
 * id:    pfm_3_flag_zero_prec_old_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (for d i o u x X the 0 flag is ignored if a precision is specified); MS docs printf flag directives
 * note:  PROBE: whether the pre-UCRT CRTs (notably msvcrt.dll / msvcr80) also ignore the '0' flag when an integer precision is given
 *        ("%05.3d" -> "  007") and treat "%05.0d" of 0 as all spaces. The standard-conforming answer is asserted; the ucrtbase
 *        equivalent lives in pfm_3_flag_zero_minus_override.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int v7 = 7, vm7 = -7, v0 = 0;
static volatile unsigned u255 = 255;

TC_CASE(zero_ignored_with_prec)
{
    char b[96];
    sprintf(b, "[%05.3d][%05.3d][%08.3x][%06.2u]", v7, vm7, u255, v7);
    CHECK_STR(b, "[  007][ -007][     0ff][    07]");
}
TC_CASE(zero_with_prec_zero_value)
{
    char b[64];
    sprintf(b, "[%05.0d][%05.d]", v0, v0);
    CHECK_STR(b, "[     ][     ]");
}
TC_CASE(wide_zero_ignored_with_prec)
{
    wchar_t b[64];
    _snwprintf(b, 64, L"[%05.3d]", v7);
    CHECK_WSTR(b, L"[  007]");
}

int main(void)
{
    tc_begin();
    TC_RUN(zero_ignored_with_prec); TC_RUN(zero_with_prec_zero_value); TC_RUN(wide_zero_ignored_with_prec);
    return tc_end();
}
