/* TC
 * id:    loc_mbstowcs_dbcs_932
 * area:  locale
 * funcs: mbstowcs setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  observed
 * ref:   Microsoft code page 932 (Shift-JIS) table
 * note:  Skipped if the .932 locale is unavailable.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char s_hira[] = {(char)0x82, (char)0xA0, 0};
static const char s_kana[] = {(char)0xA1, 0};
static const char s_mix[] = {'a', (char)0x82, (char)0xA0, 'b', 0};
static const char *volatile p_hira = s_hira, *volatile p_kana = s_kana, *volatile p_mix = s_mix;

TC_CASE(hiragana_a) { wchar_t w[4]; w[1] = 0x7777; CHECK_EQ_U(mbstowcs(w, p_hira, 4), 1); CHECK_EQ_U(w[0], 0x3042); CHECK_EQ_U(w[1], 0); }
TC_CASE(halfwidth_kana) { wchar_t w[4]; CHECK_EQ_U(mbstowcs(w, p_kana, 4), 1); CHECK_EQ_U(w[0], 0xFF61); }
TC_CASE(mixed_ascii_dbcs) { wchar_t w[8]; CHECK_EQ_U(mbstowcs(w, p_mix, 8), 3); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 0x3042); CHECK_EQ_U(w[2], 'b'); CHECK_EQ_U(w[3], 0); }
TC_CASE(count_dest_null) { CHECK_EQ_U(mbstowcs(NULL, p_mix, 0), 3); CHECK_EQ_U(mbstowcs(NULL, p_hira, 0), 1); }

int main(void)
{
    tc_begin();
    if (!setlocale(LC_ALL, ".932")) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID "::all .932 unsupported\n"); return tc_end(); }
    TC_RUN(hiragana_a); TC_RUN(halfwidth_kana); TC_RUN(mixed_ascii_dbcs); TC_RUN(count_dest_null);
    setlocale(LC_ALL, "C");
    return tc_end();
}
