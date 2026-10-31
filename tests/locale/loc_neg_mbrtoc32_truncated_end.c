/* TC
 * id:    loc_neg_mbrtoc32_truncated_end
 * area:  locale
 * funcs: mbrtoc32 setlocale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.28.1.2 mbrtoc32 ((size_t)-2: n bytes form an incomplete but potentially valid character)
 * note:  Truncation is not an encoding error: result is -2, not -1.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(partial_four_byte_minus2) { mbstate_t st; char32_t c = 0; static const char s[] = "\xF0\x9F\x98\x80"; if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 2, &st), (size_t)-2);
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 3, &st), (size_t)-2); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(partial_four_byte_minus2); return tc_end(); }
