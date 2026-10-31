/* TC
 * id:    pfw_neg_fwprintf_after_unencodable_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _wfopen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.29.2.2
 * note:  PROBES: after an encoding failure on an ANSI stream (U+4E2D in C locale), a later fwprintf of plain ASCII on the same
 *        stream succeeds (returns its length). Unknown whether the stream is left in an error state.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_fwprintf_after_unencodable_probe_a.tmp"
static volatile wchar_t zh = 0x4E2D;

TC_CASE(ascii_after)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    r = fwprintf(f, L"%c", zh);
    CHECK_EQ_I(r, -1);
    r = fwprintf(f, L"abc");
    CHECK_EQ_I(r, 3);
    fclose(f); _wunlink(FN);
}
int main(void) { tc_begin(); TC_RUN(ascii_after); return tc_end(); }
