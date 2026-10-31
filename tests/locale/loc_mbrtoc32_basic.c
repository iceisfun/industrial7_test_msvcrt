/* TC
 * id:    loc_mbrtoc32_basic
 * area:  locale
 * funcs: mbrtoc32 setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.28.1.2 mbrtoc32; MS docs mbrtoc32
 * note:  Non-ASCII cases skipped when setlocale(LC_ALL, ".UTF-8") fails.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(ascii_c_locale) { mbstate_t st; char32_t c = 0; volatile const char *s = "A"; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, (const char *)s, 1, &st), 1); CHECK_EQ_U(c, U'A'); }
TC_CASE(two_byte) { mbstate_t st; char32_t c = 0; static const char s[] = "\xC3\xA9"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 2, &st), 2); CHECK_EQ_U(c, 0xE9); setlocale(LC_ALL, "C"); }
TC_CASE(three_byte) { mbstate_t st; char32_t c = 0; static const char s[] = "\xE2\x82\xAC"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 3, &st), 3); CHECK_EQ_U(c, 0x20AC); setlocale(LC_ALL, "C"); }
TC_CASE(four_byte_supplementary) { mbstate_t st; char32_t c = 0; static const char s[] = "\xF0\x9F\x98\x80"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 4, &st), 4); CHECK_EQ_U(c, 0x1F600); setlocale(LC_ALL, "C"); }
TC_CASE(nul_returns_0) { mbstate_t st; char32_t c = 0x55; volatile const char *s = ""; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, (const char *)s, 1, &st), 0); CHECK_EQ_U(c, 0); }
int main(void) { tc_begin(); TC_RUN(ascii_c_locale); TC_RUN(two_byte); TC_RUN(three_byte); TC_RUN(four_byte_supplementary); TC_RUN(nul_returns_0); return tc_end(); }
