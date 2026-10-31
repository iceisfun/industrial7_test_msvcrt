/* TC
 * id:    pfr_neg_probe_sprintf_wc_ilseq
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _snprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf %lc/%ls; wctomb EILSEQ
 * note:  PROBE: in the "C" locale a wide char above 0xFF cannot convert to a narrow char: output call returns -1 with
 *        errno EILSEQ. Values 0x80-0xFF are deliberately not asserted. Handler is installed in case it is invoked.
 *        Narrowed to ucrtbase: pre-UCRT CRTs may skip the %lc char (no_output) rather than failing the whole call.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(lc_sprintf)
{
    char b[32]; const char * volatile f = "%lc"; volatile wchar_t w = 0x4E2D; int r;
    errno = 0; r = sprintf(b, f, (int)w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
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
