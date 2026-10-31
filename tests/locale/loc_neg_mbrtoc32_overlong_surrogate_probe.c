/* TC
 * id:    loc_neg_mbrtoc32_overlong_surrogate_probe
 * area:  locale
 * funcs: mbrtoc32 setlocale
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   Unicode UTF-8 well-formedness (Table 3-7); C11 7.28.1.2 EILSEQ
 * note:  PROBE: whether the ucrt UTF-8 decoder rejects overlong C0 80, encoded surrogate ED A0 80 and F4 90 80 80 (> U+10FFFF) with -1/EILSEQ. Believed strict; not documented by MS.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }
static void bad(const char *s, size_t n)
{
    mbstate_t st; char32_t c = 0; size_t r;
    if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); }
    memset(&st, 0, sizeof st); errno = 0; r = mbrtoc32(&c, s, n, &st);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(overlong_c0_80) { bad("\xC0\x80", 2); }
TC_CASE(encoded_surrogate_eda080) { bad("\xED\xA0\x80", 3); }
TC_CASE(above_10ffff) { bad("\xF4\x90\x80\x80", 4); }
int main(void) { tc_begin(); TC_RUN(overlong_c0_80); TC_RUN(encoded_surrogate_eda080); TC_RUN(above_10ffff); return tc_end(); }
