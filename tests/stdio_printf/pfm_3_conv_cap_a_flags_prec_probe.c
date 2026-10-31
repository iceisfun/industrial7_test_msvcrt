/* TC
 * id:    pfm_3_conv_cap_a_flags_prec_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1p4 (bare '.' = precision 0), p6 flags; MS docs printf precision
 * note:  PROBING: %A with + / space flags and bare-dot precision (0X1P+0) on ucrtbase and msvcr120 (msvcr120 %a formatting is not documented in detail). Explicit .1 used for the flag cases to avoid the default-precision question; bare '.' is precision 0 per C and MS docs
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double one = 1.0;
#define N(x) (sizeof(x) / sizeof((x)[0]))
TC_CASE(plus) { char b[64]; sprintf(b, "[%+.1A]", one); CHECK_STR(b, "[+0X1.0P+0]"); }
TC_CASE(space) { char b[64]; sprintf(b, "[% .1A]", one); CHECK_STR(b, "[ 0X1.0P+0]"); }
TC_CASE(bare_dot) { char b[64]; sprintf(b, "[%.A]", one); CHECK_STR(b, "[0X1P+0]"); }
TC_CASE(wide)
{
    wchar_t b[64];
    _snwprintf(b, N(b), L"[%+.1A][% .1A][%.A]", one, one, one);
    CHECK_WSTR(b, L"[+0X1.0P+0][ 0X1.0P+0][0X1P+0]");
}
int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(space); TC_RUN(bare_dot); TC_RUN(wide); return tc_end(); }
