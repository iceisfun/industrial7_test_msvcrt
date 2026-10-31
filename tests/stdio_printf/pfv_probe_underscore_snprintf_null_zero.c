/* TC
 * id:    pfv_probe_underscore_snprintf_null_zero
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _snprintf (UCRT count-only mode, unverified)
 * note:  PROBE: ucrt _snprintf(NULL,0,"abc") returns 3 (count-only) rather than -1. Real Windows decides.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

TC_CASE(null_zero_returns_len) { volatile int n = _snprintf(NULL, 0, "%s", "abc"); CHECK_EQ_I(n, 3); }

int main(void) { tc_begin(); TC_RUN(null_zero_returns_len); return tc_end(); }
