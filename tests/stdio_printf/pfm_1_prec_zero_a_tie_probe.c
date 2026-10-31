/* TC
 * id:    pfm_1_prec_zero_a_tie_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 (a, A); MS docs printf type field a
 * note:  PROBE: hex-float %.0a of tie 1.5 (0x1.8p+0): ISO C leaves rounding of hex digits implementation-defined; guess ucrt rounds to even -> 0x2p+0.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define T(fmt, v, want) do { char b[64]; volatile double tv = (v); sprintf(b, fmt, tv); CHECK_STR(b, want); } while (0)

TC_CASE(tie) { T("%.0a", 1.5, "0x2p+0"); T("%.a", 1.5, "0x2p+0"); }

int main(void) { tc_begin(); TC_RUN(tie); return tc_end(); }
