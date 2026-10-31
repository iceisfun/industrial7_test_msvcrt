/* TC
 * id:    pfw_probe_wide_width_prec_surrogate
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf width/precision count "characters"; wide CRT is expected to count wchar_t code units
 * note:  Probes that width padding and precision truncation in a wide printf count UTF-16 code units, not code points:
 *        %5s of a surrogate pair pads 3, %.1s of a pair emits only the high surrogate. Not stated by the docs.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile pair = L"\xD83D\xDE00";
TC_CASE(w5s_with_pair_3_pad) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%5s", pair); CHECK_EQ_I(n, 5); CHECK(b[0]==L' ' && b[1]==L' ' && b[2]==L' '); CHECK_EQ_U(b[3], 0xD83D); CHECK_EQ_U(b[4], 0xDE00); }
TC_CASE(wm5s_with_pair) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%-5s|", pair); CHECK_EQ_I(n, 6); CHECK_EQ_U(b[0], 0xD83D); CHECK_EQ_U(b[1], 0xDE00); CHECK(b[2]==L' ' && b[3]==L' ' && b[4]==L' '); CHECK_EQ_U(b[5], '|'); }
TC_CASE(p1s_on_pair_high_only) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%.1s", pair); CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 0xD83D); }
int main(void) { tc_begin(); TC_RUN(w5s_with_pair_3_pad); TC_RUN(wm5s_with_pair); TC_RUN(p1s_on_pair_high_only); return tc_end(); }
