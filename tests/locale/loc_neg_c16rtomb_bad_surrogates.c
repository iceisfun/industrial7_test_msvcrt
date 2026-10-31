/* TC
 * id:    loc_neg_c16rtomb_bad_surrogates
 * area:  locale
 * funcs: c16rtomb setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.28.1.3 c16rtomb (EILSEQ when c16 is not a valid UTF-16 sequence); Unicode UTF-16 well-formedness
 * note:  Needs the UTF-8 locale (Windows 10 1803+); skipped otherwise. After an error the conversion state is
 *        unspecified, so recovery is checked with a fresh mbstate_t.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile unsigned int v_u;
#define BAD ((size_t)-1)

static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }
static void zero(mbstate_t *s) { size_t i; for (i = 0; i < sizeof *s; i++) ((char *)s)[i] = 0; }
static size_t c16(char *b, unsigned int u, mbstate_t *s) { v_u = u; return c16rtomb(b, (char16_t)v_u, s); }

TC_CASE(lone_low)
{
    mbstate_t st; char b[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    zero(&st);
    CHECK_EQ_U(c16(b, 0xDC00, &st), BAD);
    setlocale(LC_ALL, "C");
}
TC_CASE(high_then_ascii)
{
    mbstate_t st; char b[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    zero(&st);
    CHECK_EQ_U(c16(b, 0xD83D, &st), 0);   /* high surrogate alone produces no output yet */
    CHECK_EQ_U(c16(b, 'A', &st), BAD);
    setlocale(LC_ALL, "C");
}
TC_CASE(errno_eilseq)
{
    mbstate_t st; char b[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    zero(&st); errno = 0;
    CHECK_EQ_U(c16(b, 0xDC00, &st), BAD);
    CHECK_EQ_I(errno, EILSEQ);
    zero(&st); c16(b, 0xD83D, &st); errno = 0;
    CHECK_EQ_U(c16(b, 'A', &st), BAD);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(state_recovers)
{
    mbstate_t st; char b[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    zero(&st); c16(b, 0xDC00, &st);
    zero(&st);
    CHECK_EQ_U(c16(b, 'A', &st), 1); CHECK_EQ_I(b[0], 'A');
    /* a well-formed pair U+1F600 -> F0 9F 98 80 still works afterwards */
    CHECK_EQ_U(c16(b, 0xD83D, &st), 0);
    CHECK_EQ_U(c16(b, 0xDE00, &st), 4);
    CHECK_EQ_U((unsigned char)b[0], 0xF0); CHECK_EQ_U((unsigned char)b[1], 0x9F);
    CHECK_EQ_U((unsigned char)b[2], 0x98); CHECK_EQ_U((unsigned char)b[3], 0x80);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(lone_low); TC_RUN(high_then_ascii); TC_RUN(errno_eilseq); TC_RUN(state_recovers);
    setlocale(LC_ALL, "C");
    return tc_end();
}
