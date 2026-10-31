/* TC
 * id:    pfw_wide_surrogate_code_units
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snwprintf: return value and count are in wide characters (wchar_t code units)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile pair = L"\xD83D\xDE00";
static const wchar_t * volatile emb = L"a\xD83D\xDE00z";
TC_CASE(c_high_then_c_low_two_units) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%c%c", 0xD83D, 0xDE00); CHECK_EQ_I(n, 2); CHECK_EQ_U(b[0], 0xD83D); CHECK_EQ_U(b[1], 0xDE00); CHECK_EQ_U(b[2], 0); }
TC_CASE(s_pair_string_ret_2) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%s", pair); CHECK_EQ_I(n, 2); CHECK_EQ_U(b[0], 0xD83D); CHECK_EQ_U(b[1], 0xDE00); CHECK_EQ_U(b[2], 0); }
TC_CASE(embedded_pair_roundtrip_bits) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"<%s>", emb); CHECK_EQ_I(n, 6);
    CHECK_EQ_U(b[0], '<'); CHECK_EQ_U(b[1], 'a'); CHECK_EQ_U(b[2], 0xD83D); CHECK_EQ_U(b[3], 0xDE00); CHECK_EQ_U(b[4], 'z'); CHECK_EQ_U(b[5], '>'); CHECK_EQ_U(b[6], 0); }
TC_CASE(ret_counts_code_units_not_codepoints) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%s%s%s", pair, pair, pair); CHECK_EQ_I(n, 6); CHECK_EQ_U(b[5], 0xDE00); CHECK_EQ_U(b[6], 0); }
int main(void) { tc_begin(); TC_RUN(c_high_then_c_low_two_units); TC_RUN(s_pair_string_ret_2); TC_RUN(embedded_pair_roundtrip_bits); TC_RUN(ret_counts_code_units_not_codepoints); return tc_end(); }
