/* TC
 * id:    pfm_1_flag_minus_zs_wide_arg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf format specification (S: wide string in narrow printf; width, - flag)
 * note:  %Z is probed separately in pfm_1_width_num_z_probe.
 */
#include "tc.h"
#include <stdio.h>

static const wchar_t * volatile ws = L"ab";

TC_CASE(left) { char b[64]; sprintf(b, "[%-6S]", ws); CHECK_STR(b, "[ab    ]"); }
TC_CASE(right) { char b[64]; sprintf(b, "[%6S]", ws); CHECK_STR(b, "[    ab]"); }
TC_CASE(precision) { char b[64]; sprintf(b, "[%-6.1S]", ws); CHECK_STR(b, "[a     ]"); }
int main(void) { tc_begin(); TC_RUN(left); TC_RUN(right); TC_RUN(precision); return tc_end(); }
