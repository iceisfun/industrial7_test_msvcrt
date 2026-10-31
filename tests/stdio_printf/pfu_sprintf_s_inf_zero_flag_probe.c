/* TC
 * id:    pfu_sprintf_s_inf_zero_flag_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 p6 ('0' flag: leading zeros used for padding "except when converting an infinity or NaN")
 * note:  PROBING exactly: whether UCRT sprintf_s follows the C standard and pads inf/nan with spaces (not zeros) when the '0' flag is given. Expectation: "[   inf]" and "[   nan]".
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned long long pinf = 0x7FF0000000000000ULL, qnan = 0x7FF8000000000000ULL;
static double D(volatile unsigned long long *p) { union { unsigned long long u; double d; } x; x.u = *p; return x.d; }

TC_CASE(inf_zero_flag) { char b[32]; sprintf_s(b, 32, "[%06f]", D(&pinf)); CHECK_STR(b, "[   inf]"); }
TC_CASE(nan_zero_flag) { char b[32]; sprintf_s(b, 32, "[%06f]", D(&qnan)); CHECK_STR(b, "[   nan]"); }

int main(void) { tc_begin(); TC_RUN(inf_zero_flag); TC_RUN(nan_zero_flag); return tc_end(); }
