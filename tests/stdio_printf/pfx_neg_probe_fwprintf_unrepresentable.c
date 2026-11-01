/* TC
 * id:    pfx_neg_probe_fwprintf_unrepresentable
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fwprintf / wctomb (EILSEQ on unconvertible wide char)
 * note:  Observed on Windows 10 22H2, "C" locale: fwprintf(f, L"%lc", U+4E2D) to an ANSI-mode file returns 1 (the unconvertible character is written as '?') with
 *        errno EILSEQ, not a negative value. Deviates from C11 7.29.2.1/7.21.6.1p14 (encoding error -> negative return).
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
    CHECK_EQ_I(n, 1);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { char b[8] = {0}; size_t k = fread(b, 1, sizeof b, f); CHECK_EQ_I((int)k, 1); CHECK_EQ_I(b[0], '?'); fclose(f); }
}
int main(void)
{
    tc_begin(); remove(F); tc_iph_install();
    TC_RUN(unconvertible);
    remove(F); return tc_end();
}
