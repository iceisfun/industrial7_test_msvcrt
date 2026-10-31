/* TC
 * id:    pfx_neg_probe_fwprintf_unrepresentable
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fwprintf / wctomb (EILSEQ on unconvertible wide char)
 * note:  PROBE: in the C locale a wide char U+4E2D written by fwprintf to an ANSI-mode file cannot be converted: returns negative and errno == EILSEQ.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#define F "tc_pfx_neg_probe_fwprintf_unrepresentable_" TCV ".tmp"
TC_CASE(unconvertible)
{
    FILE *f = fopen(F, "w"); int n; volatile wchar_t wc = 0x4E2D;
    CHECK(f != NULL); if (!f) return;
    errno = 0;
    n = fwprintf(f, L"%lc", (wint_t)wc);
    CHECK(n < 0);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
}
int main(void)
{
    tc_begin(); remove(F); tc_iph_install();
    TC_RUN(unconvertible);
    remove(F); return tc_end();
}
