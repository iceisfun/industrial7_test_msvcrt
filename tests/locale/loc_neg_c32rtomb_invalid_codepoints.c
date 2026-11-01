/* TC
 * id:    loc_neg_c32rtomb_invalid_codepoints
 * area:  locale
 * funcs: c32rtomb setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.28.1.4 c32rtomb (EILSEQ if c32 is not a valid character in the current locale); Unicode range U+0000..U+10FFFF
 * note:  Win10 triage (class B): the 1252 "unmappable" cases do not fail on ucrtbase (UTF-8 output, 2 and 3 bytes); observed on Windows 10 22H2
 *        build 19045, deviates from C11 7.28.1.4. The above-U+10FFFF cases still fail with EILSEQ as asserted.
 *        Surrogate code points D800..DFFF are a separate probe (loc_probe_c32rtomb_surrogate_cp).
 *        UTF-8 cases skipped when the UTF-8 locale is unavailable. U+0416 (Cyrillic ZHE) has no Windows-1252 mapping.
 *        [class B] deviates from C11 7.28.1 (locale-encoding conversion); the MS docs state c16rtomb/c32rtomb convert to UTF-8 and mbrtoc16/mbrtoc32 decode UTF-8.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile unsigned int v_u;
#define BAD ((size_t)-1)

static void zero(mbstate_t *s) { size_t i; for (i = 0; i < sizeof *s; i++) ((char *)s)[i] = 0; }
static size_t c32(unsigned int u) { mbstate_t st; char b[16]; zero(&st); v_u = u; return c32rtomb(b, (char32_t)v_u, &st); }
static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }

TC_CASE(above_10ffff)
{
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(c32(0x110000), BAD);
    CHECK_EQ_U(c32(0x200000), BAD);
    setlocale(LC_ALL, "C");
}
TC_CASE(all_ones)
{
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(c32(0xFFFFFFFFu), BAD);
    setlocale(LC_ALL, "C");
}
TC_CASE(cp1252_unmappable)
{
    v_loc = "English_United States.1252";
    if (!setlocale(LC_ALL, v_loc)) TC_SKIP("no 1252 locale");
    /* Win10 22H2 ucrtbase (observed): c32rtomb ignores the 1252 code page and emits UTF-8, so these succeed (deviates from C11 7.28.1.4) */
    CHECK_EQ_U(c32(0x0416), 2);
    CHECK_EQ_U(c32(0x4E2D), 3);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    errno = 0; CHECK_EQ_U(c32(0x110000), BAD); CHECK_EQ_I(errno, EILSEQ);
    errno = 0; CHECK_EQ_U(c32(0xFFFFFFFFu), BAD); CHECK_EQ_I(errno, EILSEQ);
    v_loc = "English_United States.1252";
    if (setlocale(LC_ALL, v_loc)) { errno = 0; CHECK_EQ_U(c32(0x0416), 2); CHECK_EQ_I(errno, 0); }   /* observed: UTF-8 regardless of the 1252 locale */
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(above_10ffff); TC_RUN(all_ones); TC_RUN(cp1252_unmappable); TC_RUN(errno_eilseq);
    setlocale(LC_ALL, "C");
    return tc_end();
}
