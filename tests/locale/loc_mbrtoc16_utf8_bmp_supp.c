/* TC
 * id:    loc_mbrtoc16_utf8_bmp_supp
 * area:  locale
 * funcs: mbrtoc16 setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.28.1.1 mbrtoc16 (-3 = next char of earlier sequence); MS docs mbrtoc16, UTF-8 support in setlocale
 * note:  Skipped when setlocale(LC_ALL, ".UTF-8") fails (pre-1803 Windows).
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }
static void clr(mbstate_t *s) { memset(s, 0, sizeof *s); }

TC_CASE(two_byte) { mbstate_t st; char16_t c = 0; static const char s[] = "\xC3\xA9"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    clr(&st); CHECK_EQ_U(mbrtoc16(&c, s, 2, &st), 2); CHECK_EQ_U(c, 0xE9); setlocale(LC_ALL, "C"); }
TC_CASE(three_byte) { mbstate_t st; char16_t c = 0; static const char s[] = "\xE2\x82\xAC"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    clr(&st); CHECK_EQ_U(mbrtoc16(&c, s, 3, &st), 3); CHECK_EQ_U(c, 0x20AC); setlocale(LC_ALL, "C"); }
TC_CASE(four_byte_first_high_surrogate) { mbstate_t st; char16_t c = 0; static const char s[] = "\xF0\x9F\x98\x80"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    clr(&st); CHECK_EQ_U(mbrtoc16(&c, s, 4, &st), 4); CHECK_EQ_U(c, 0xD83D); setlocale(LC_ALL, "C"); }
TC_CASE(four_byte_second_minus3_low_surrogate) { mbstate_t st; char16_t c = 0, d = 0; static const char s[] = "\xF0\x9F\x98\x80\0"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    clr(&st); CHECK_EQ_U(mbrtoc16(&c, s, 4, &st), 4); CHECK_EQ_U(c, 0xD83D);
    CHECK_EQ_U(mbrtoc16(&d, s + 4, 1, &st), (size_t)-3); CHECK_EQ_U(d, 0xDE00); setlocale(LC_ALL, "C"); }
TC_CASE(state_initial_after_pair) { mbstate_t st; char16_t c = 0, d = 0; static const char s[] = "\xF0\x9F\x98\x80\0"; static const char a[] = "A"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    clr(&st); mbrtoc16(&c, s, 4, &st); mbrtoc16(&d, s + 4, 1, &st);
    c = 0; CHECK_EQ_U(mbrtoc16(&c, a, 1, &st), 1); CHECK_EQ_U(c, u'A'); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(two_byte); TC_RUN(three_byte); TC_RUN(four_byte_first_high_surrogate); TC_RUN(four_byte_second_minus3_low_surrogate); TC_RUN(state_initial_after_pair); return tc_end(); }
