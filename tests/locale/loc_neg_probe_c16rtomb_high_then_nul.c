/* TC
 * id:    loc_neg_probe_c16rtomb_high_then_nul
 * area:  locale
 * funcs: c16rtomb setlocale
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.28.1.3 c16rtomb (c16 == 0 case vs pending high surrogate in state)
 * note:  PROBE: in a UTF-8 locale, a high surrogate followed by the NUL char16 (instead of a low surrogate)
 *        should be rejected with (size_t)-1 / EILSEQ. The standard is silent on NUL arriving mid-pair.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile unsigned int v_u;
static size_t c16(char *b, unsigned int u, mbstate_t *s) { v_u = u; return c16rtomb(b, (char16_t)v_u, s); }

TC_CASE(high_then_nul)
{
    mbstate_t st; char b[16]; size_t i;
    v_loc = ".UTF-8"; if (!setlocale(LC_ALL, v_loc)) TC_SKIP("no UTF-8 locale");
    for (i = 0; i < sizeof st; i++) ((char *)&st)[i] = 0;
    CHECK_EQ_U(c16(b, 0xD83D, &st), 0);
    errno = 0;
    CHECK_EQ_U(c16(b, 0, &st), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(high_then_nul); setlocale(LC_ALL, "C"); return tc_end(); }
