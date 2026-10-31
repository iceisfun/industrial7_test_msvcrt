/* TC
 * id:    pfx_probe_neg_fwprintf_eilseq
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fwprintf (EILSEQ on encoding errors)
 * note:  PROBE: in the C locale a wide char above 0xFF (U+0100) written to a narrow (ANSI) text stream
 *        fails with -1 / EILSEQ. U+0100 is used rather than U+00E9 because the C-locale wctomb path is believed
 *        to map 0x80-0xFF one-to-one (cast to char) in several CRTs, so only > 0xFF is a likely error everywhere.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile path = "tc_pfx_probe_neg_fwprintf_eilseq_a.tmp";

TC_CASE(eilseq)
{
    FILE *f = fopen(path, "w");
    volatile int n;
    CHECK(f != NULL);
    if (!f) return;
    errno = 0;
    n = fwprintf(f, L"\x0100");
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
    DeleteFileA(path);
}
int main(void) { tc_begin(); TC_RUN(eilseq); return tc_end(); }
