/* TC
 * id:    pfi_probe_snwprintf_prec_zero_flag_old
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 p6 (0 flag ignored when a precision is given)
 * note:  PROBE: in the wide printf of Win7-era CRTs an explicit precision overrides the 0 flag, as the standard says.
 *        Expected yes; the author is not certain for msvcrt.dll/msvcr80.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(prec_over_zero) { wchar_t b[64]; _snwprintf(b, 64, L"[%08.3d][%08.3d]", 7, -7); CHECK_WSTR(b, L"[     007][    -007]"); }
int main(void) { tc_begin(); TC_RUN(prec_over_zero); return tc_end(); }
