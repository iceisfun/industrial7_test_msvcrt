/* TC
 * id:    pfw_neg_fwprintf_after_unencodable_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _wfopen
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.29.2.2
 * note:  Observed on Windows 10 22H2, "C" locale: fwprintf(f, L"%c", U+4E2D) on an ANSI-mode file stream returns 1 (the unencodable character is written as
 *        '?', errno EILSEQ) and does not poison the stream: a later fwprintf of ASCII returns 3. Deviates from C11 7.29.2.1 / 7.21.6.1p14 (encoding error -> negative).
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
    CHECK_EQ_I(r, 1);
    r = fwprintf(f, L"abc");
    CHECK_EQ_I(r, 3);
    fclose(f);
    f = _wfopen(FN, L"rb");
    if (f) { char b[8] = {0}; size_t k = fread(b, 1, sizeof b, f); CHECK_EQ_I((int)k, 4); CHECK_STR(b, "?abc"); fclose(f); }
    _wunlink(FN);
}
int main(void) { tc_begin(); TC_RUN(ascii_after); return tc_end(); }
