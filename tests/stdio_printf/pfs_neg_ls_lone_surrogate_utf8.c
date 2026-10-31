/* TC
 * id:    pfs_neg_ls_lone_surrogate_utf8
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs setlocale (UTF-8 support, UCRT); printf %ls / %lc
 * note:  PROBE: with a UTF-8 locale, converting a lone UTF-16 surrogate in %ls / %lc cannot produce valid UTF-8, so sprintf is
 *        expected to fail with -1 and errno EILSEQ. Skipped if the machine rejects ".UTF-8".
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>
#include <errno.h>

static const wchar_t * volatile lone_hi = L"\xD800";
static volatile wchar_t lone_lo = 0xDC00;

TC_CASE(ls_lone_high)
{
    char b[32]; int r;
    if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("no UTF-8 locale");
    errno = 0; r = sprintf(b, "%ls", lone_hi);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(lc_lone_low)
{
    char b[32]; int r;
    if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("no UTF-8 locale");
    errno = 0; r = sprintf(b, "%lc", (wint_t)lone_lo);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(ls_lone_high); TC_RUN(lc_lone_low); setlocale(LC_ALL, "C"); return tc_end(); }
