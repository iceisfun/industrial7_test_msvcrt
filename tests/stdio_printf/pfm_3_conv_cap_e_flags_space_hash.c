/* TC
 * id:    pfm_3_conv_cap_e_flags_space_hash
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p6 flags space and #; MS docs printf flag directives
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <string.h>

static volatile double d15 = 1.5, d3 = 3.0, dm2 = -2.0;
#define N(x) (sizeof(x) / sizeof((x)[0]))
#if TC_AT_LEAST(140)
#define X "+00"
#else
#define X "+000"
#endif
TC_CASE(space) { char b[64]; int n = sprintf(b, "[% E]", d15); CHECK_STR(b, "[ 1.500000E" X "]"); CHECK_EQ_I(n, (int)strlen(b)); }
TC_CASE(hash_p0) { char b[64]; sprintf(b, "[%#.0E]", d3); CHECK_STR(b, "[3.E" X "]"); }
TC_CASE(space_hash_neg) { char b[64]; sprintf(b, "[%# E]", dm2); CHECK_STR(b, "[-2.000000E" X "]"); }
TC_CASE(wide)
{
    wchar_t b[64];
    _snwprintf(b, N(b), L"[% E][%#.0E][%# E]", d15, d3, dm2);
#if TC_AT_LEAST(140)
    CHECK_WSTR(b, L"[ 1.500000E+00][3.E+00][-2.000000E+00]");
#else
    CHECK_WSTR(b, L"[ 1.500000E+000][3.E+000][-2.000000E+000]");
#endif
}
int main(void) { tc_begin(); TC_RUN(space); TC_RUN(hash_p0); TC_RUN(space_hash_neg); TC_RUN(wide); return tc_end(); }
