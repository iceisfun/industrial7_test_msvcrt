/* TC
 * id:    pfm_1_star_width_chars_strings
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1; MS docs printf width specification and type field (C, S: wide in narrow printf)
 * note:  %*p is probed separately in pfm_1_star_width_pointer_probe.
 */
#include "tc.h"
#include <stdio.h>

static const wchar_t * volatile ws = L"ab";

TC_CASE(c) { char b[64]; sprintf(b, "[%*c]", 4, 'x'); CHECK_STR(b, "[   x]"); }
TC_CASE(C) { char b[64]; sprintf(b, "[%*C]", 4, (wint_t)L'x'); CHECK_STR(b, "[   x]"); }
TC_CASE(s) { char b[64]; sprintf(b, "[%*s]", 6, "ab"); CHECK_STR(b, "[    ab]"); }
TC_CASE(S) { char b[64]; sprintf(b, "[%*S]", 6, ws); CHECK_STR(b, "[    ab]"); }
TC_CASE(s_minus) { char b[64]; sprintf(b, "[%-*s]", 6, "ab"); CHECK_STR(b, "[ab    ]"); }
TC_CASE(s_negative) { char b[64]; sprintf(b, "[%*s]", -6, "ab"); CHECK_STR(b, "[ab    ]"); }
int main(void) { tc_begin(); TC_RUN(c); TC_RUN(C); TC_RUN(s); TC_RUN(S); TC_RUN(s_minus); TC_RUN(s_negative); return tc_end(); }
