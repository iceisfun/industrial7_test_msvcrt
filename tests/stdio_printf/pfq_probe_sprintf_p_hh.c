/* TC
 * id:    pfq_probe_sprintf_p_hh
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  PROBES the 'hh' length modifier with a positional spec: %1$hhd of 300 -> 44 (signed char truncation). hh is a C99 addition supported by UCRT; older CRTs unsure so ucrtbase only.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$hhd";
static const char * volatile f2 = "%1$hhu";

TC_CASE(hhd) { char b[32]; _sprintf_p(b, sizeof b, f1, 300); CHECK_STR(b, "44"); }
TC_CASE(hhu) { char b[32]; _sprintf_p(b, sizeof b, f2, 300); CHECK_STR(b, "44"); }

int main(void) { tc_begin(); TC_RUN(hhd); TC_RUN(hhu); return tc_end(); }
