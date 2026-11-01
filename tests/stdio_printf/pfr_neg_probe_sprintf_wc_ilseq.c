/* TC
 * id:    pfr_neg_probe_sprintf_wc_ilseq
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf %lc/%ls; wctomb EILSEQ
 * note:  Observed on Windows 10 22H2, "C" locale: sprintf("%lc", U+4E2D) sets errno EILSEQ but returns 0 (empty successful output) instead of -1;
 *        %ls / _snprintf %ls of the same char return -1/EILSEQ. Deviates from C11 7.21.6.1p14 (encoding error -> negative return).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(lc_sprintf)
{
    char b[32]; const char * volatile f = "%lc"; volatile wchar_t w = 0x4E2D; int r;
    errno = 0; r = sprintf(b, f, (int)w);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(ls_sprintf)
{
    static const wchar_t ws[] = { 0x4E2D, 0 }; const wchar_t * volatile p = ws;
    char b[32]; const char * volatile f = "%ls"; int r;
    errno = 0; r = sprintf(b, f, p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(ls_snprintf)
{
    static const wchar_t ws[] = { 0x4E2D, 0 }; const wchar_t * volatile p = ws;
    char b[32]; const char * volatile f = "%ls"; int r;
    errno = 0; r = _snprintf(b, sizeof b, f, p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(lc_sprintf); TC_RUN(ls_sprintf); TC_RUN(ls_snprintf); return tc_end();
}
