/* TC
 * id:    pfm_2_neg_swprintf_trunc_nul_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs swprintf (UCRT); C11 7.29.2.3 (output beyond n-1 is discarded, null wide char appended)
 * note:  PROBES that on truncation ucrt swprintf writes exactly n-1 characters followed by a NUL, writes nothing past index n-1,
 *        and returns a negative value.
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(trunc_terminates)
{
    wchar_t b[12]; int i, n; const wchar_t * volatile f = L"%ls";
    for (i = 0; i < 12; i++) b[i] = L'Q';
    n = swprintf(b, 4, f, L"abcdefgh");
    CHECK(n < 0);
    CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[1], L'b'); CHECK_EQ_I(b[2], L'c'); CHECK_EQ_I(b[3], 0);
    for (i = 4; i < 12; i++) CHECK_EQ_I(b[i], L'Q');
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(trunc_terminates); return tc_end(); }
