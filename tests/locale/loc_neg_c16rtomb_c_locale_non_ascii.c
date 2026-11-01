/* TC
 * id:    loc_neg_c16rtomb_c_locale_non_ascii
 * area:  locale
 * funcs: c16rtomb c32rtomb
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.28.1.3/7.28.1.4 (EILSEQ for characters not representable in the current locale)
 * note:  Win10 triage (class B): observed on Windows 10 22H2 build 19045, c16rtomb(U+0100)=2, c32rtomb(U+20AC)=3, errno untouched, in the C locale
 *        (UTF-8 output regardless of locale; deviates from C11 7.28.1.3/4 which wants EILSEQ). Original probe text follows.
 *        PROBE: in the "C" locale U+0100 and U+20AC cannot be represented and should give (size_t)-1/EILSEQ.
 *        U+00E9 is only printed as a diagnostic (UCRT's C locale may map 0x80..0xFF 1:1 or reject them); not asserted.
 *        [class B] deviates from C11 7.28.1 (locale-encoding conversion); the MS docs state c16rtomb/c32rtomb convert to UTF-8 and mbrtoc16/mbrtoc32 decode UTF-8.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

static volatile unsigned int v_u;
static void zero(mbstate_t *s) { size_t i; for (i = 0; i < sizeof *s; i++) ((char *)s)[i] = 0; }
static size_t c16(unsigned int u) { mbstate_t st; char b[16]; zero(&st); v_u = u; return c16rtomb(b, (char16_t)v_u, &st); }
static size_t c32(unsigned int u) { mbstate_t st; char b[16]; zero(&st); v_u = u; return c32rtomb(b, (char32_t)v_u, &st); }

/* Win10 22H2 ucrtbase (observed): c16rtomb/c32rtomb always encode UTF-8 even in the "C" locale, so nothing here fails (deviates from C11 7.28.1.3/4). */
TC_CASE(c16_u0100) { setlocale(LC_ALL, "C"); CHECK_EQ_U(c16(0x0100), 2); }
TC_CASE(c32_u20ac) { setlocale(LC_ALL, "C"); CHECK_EQ_U(c32(0x20AC), 3); }
/* name kept for case-id stability: no EILSEQ on Windows, errno stays 0 */
TC_CASE(errno_eilseq)
{
    setlocale(LC_ALL, "C");
    errno = 0; c16(0x0100); CHECK_EQ_I(errno, 0);
    errno = 0; c32(0x20AC); CHECK_EQ_I(errno, 0);
}
TC_CASE(diag_e9)
{
    char b[24]; size_t r; setlocale(LC_ALL, "C");
    r = c32(0xE9);
    CHECK_EQ_U(r, 2);   /* observed: UTF-8 C3 A9 */
    tc_write("TC INFO C-locale c32rtomb(U+00E9) = "); tc_i64((long long)r, b); tc_write(b); tc_write("\n");
}
int main(void) { tc_begin(); TC_RUN(c16_u0100); TC_RUN(c32_u20ac); TC_RUN(errno_eilseq); TC_RUN(diag_e9); setlocale(LC_ALL, "C"); return tc_end(); }
