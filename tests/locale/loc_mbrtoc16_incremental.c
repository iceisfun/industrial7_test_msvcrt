/* TC
 * id:    loc_mbrtoc16_incremental
 * area:  locale
 * funcs: mbrtoc16 setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.28.1.1 mbrtoc16 ((size_t)-2 for an incomplete but potentially valid sequence)
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static mbstate_t st; static char16_t c; static const char s[] = "\xE2\x82\xAC";
static int begin(void) { volatile const char *n = ".UTF-8"; memset(&st, 0, sizeof st); c = 0; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(first_byte_minus2) { if (!begin()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); } CHECK_EQ_U(mbrtoc16(&c, s, 1, &st), (size_t)-2); setlocale(LC_ALL, "C"); }
TC_CASE(second_byte_minus2) { if (!begin()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    CHECK_EQ_U(mbrtoc16(&c, s, 1, &st), (size_t)-2); CHECK_EQ_U(mbrtoc16(&c, s + 1, 1, &st), (size_t)-2); setlocale(LC_ALL, "C"); }
TC_CASE(third_byte_1_value) { if (!begin()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    mbrtoc16(&c, s, 1, &st); mbrtoc16(&c, s + 1, 1, &st);
    CHECK_EQ_U(mbrtoc16(&c, s + 2, 1, &st), 1); CHECK_EQ_U(c, 0x20AC); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(first_byte_minus2); TC_RUN(second_byte_minus2); TC_RUN(third_byte_1_value); return tc_end(); }
