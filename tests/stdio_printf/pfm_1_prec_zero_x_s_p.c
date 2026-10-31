/* TC
 * id:    pfm_1_prec_zero_x_s_p
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p8; MS docs printf precision
 * note:  name kept from spec; %p moved to pfm_1_bare_dot_p_probe. %S in sprintf is a wide string.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(X) {
    char b[32]; volatile unsigned z = 0, v = 255;
    sprintf(b, "[%.0X]", z); CHECK_STR(b, "[]");
    sprintf(b, "[%.0X]", v); CHECK_STR(b, "[FF]");
    sprintf(b, "[%#.0X]", z); CHECK_STR(b, "[]");
    sprintf(b, "[%#.0X]", v); CHECK_STR(b, "[0XFF]");
    sprintf(b, "[%4.0X]", z); CHECK_STR(b, "[    ]");
}
TC_CASE(S) {
    char b[32]; const wchar_t * volatile ws = L"abc";
    sprintf(b, "[%.0S]", ws); CHECK_STR(b, "[]");
    sprintf(b, "[%.0ls]", ws); CHECK_STR(b, "[]");
    sprintf(b, "[%4.0S]", ws); CHECK_STR(b, "[    ]");
}

int main(void) { tc_begin(); TC_RUN(X); TC_RUN(S); return tc_end(); }
