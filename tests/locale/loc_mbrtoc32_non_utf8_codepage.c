/* TC
 * id:    loc_mbrtoc32_non_utf8_codepage
 * area:  locale
 * funcs: mbrtoc32 mbrtoc16 setlocale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.28.1.1/7.28.1.2 (conversion per the current locale); MS docs mbrtoc16, mbrtoc32
 * note:  PROBE: whether ucrt mbrtoc16/mbrtoc32 honour the current non-UTF-8 locale code page (1252, 932) or always decode UTF-8
 *        regardless of locale (some reports suggest the latter). Not provable from the docs; real Windows decides.
 *        Skipped when the code-page locale is rejected.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static int loc(const char *n) { volatile const char *v = n; if (setlocale(LC_ALL, (const char *)v)) return 1; setlocale(LC_ALL, "C"); return 0; }

TC_CASE(cp1252_e9) { mbstate_t st; char32_t c = 0; static const char s[] = "\xE9"; if (!loc(".1252")) TC_SKIP("1252 rejected");
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 1, &st), 1); CHECK_EQ_U(c, 0xE9); setlocale(LC_ALL, "C"); }
TC_CASE(cp1252_euro) { mbstate_t st; char32_t c = 0; static const char s[] = "\x80"; if (!loc(".1252")) TC_SKIP("1252 rejected");
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 1, &st), 1); CHECK_EQ_U(c, 0x20AC); setlocale(LC_ALL, "C"); }
TC_CASE(cp932_pair) { mbstate_t st; char32_t c = 0; static const char s[] = "\x82\xA0"; if (!loc(".932")) TC_SKIP("932 rejected");
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc32(&c, s, 2, &st), 2); CHECK_EQ_U(c, 0x3042); setlocale(LC_ALL, "C"); }
TC_CASE(mbrtoc16_equivalent) { mbstate_t st; char16_t c = 0; static const char s[] = "\x82\xA0"; static const char e[] = "\x80";
    if (!loc(".932")) TC_SKIP("932 rejected");
    memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc16(&c, s, 2, &st), 2); CHECK_EQ_U(c, 0x3042);
    if (!loc(".1252")) TC_SKIP("1252 rejected");
    memset(&st, 0, sizeof st); c = 0; CHECK_EQ_U(mbrtoc16(&c, e, 1, &st), 1); CHECK_EQ_U(c, 0x20AC); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(cp1252_e9); TC_RUN(cp1252_euro); TC_RUN(cp932_pair); TC_RUN(mbrtoc16_equivalent); return tc_end(); }
