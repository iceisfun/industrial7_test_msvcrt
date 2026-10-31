/* TC
 * id:    pfw_neg_fwprintf_unencodable_ansi
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf _wfopen
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.29.2.2 (encoding error -> negative return); MS: wide output to an ANSI-mode stream converts via wctomb; failure sets EILSEQ
 * note:  C locale: U+4E2D has no single-byte representation. PROBED: return -1 and errno == EILSEQ on every CRT
 *        (old-CRT errno/return for a failed wide->narrow conversion is not documented).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_fwprintf_unencodable_ansi_a.tmp"
static volatile wchar_t zh = 0x4E2D;
static wchar_t zhs[3];

TC_CASE(percent_c)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    errno = 0; r = fwprintf(f, L"%c", zh);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
    fclose(f); _wunlink(FN);
}
TC_CASE(percent_ls)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    zhs[0] = L'a'; zhs[1] = zh; zhs[2] = 0;
    errno = 0; r = fwprintf(f, L"%ls", zhs);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
    fclose(f); _wunlink(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(percent_c); TC_RUN(percent_ls);
    return tc_end();
}
