/* TC
 * id:    loc_probe_c32rtomb_surrogate_cp
 * area:  locale
 * funcs: c32rtomb setlocale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   Unicode: D800..DFFF are not scalar values; C11 7.28.1.4
 * note:  PROBE: whether c32rtomb in a UTF-8 locale rejects surrogate code points D800 and DFFF with (size_t)-1 / EILSEQ
 *        (strict scalar-value check) rather than encoding them as 3-byte CESU-style sequences.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile unsigned int v_u;
static size_t c32(unsigned int u) { mbstate_t st; char b[16]; size_t i; for (i = 0; i < sizeof st; i++) ((char *)&st)[i] = 0; v_u = u; return c32rtomb(b, (char32_t)v_u, &st); }

TC_CASE(d800)
{
    v_loc = ".UTF-8"; if (!setlocale(LC_ALL, v_loc)) TC_SKIP("no UTF-8 locale");
    errno = 0; CHECK_EQ_U(c32(0xD800), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(dfff)
{
    v_loc = ".UTF-8"; if (!setlocale(LC_ALL, v_loc)) TC_SKIP("no UTF-8 locale");
    errno = 0; CHECK_EQ_U(c32(0xDFFF), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(d800); TC_RUN(dfff); setlocale(LC_ALL, "C"); return tc_end(); }
