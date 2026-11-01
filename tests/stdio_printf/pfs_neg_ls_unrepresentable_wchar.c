/* TC
 * id:    pfs_neg_ls_unrepresentable_wchar
 * area:  stdio_printf
 * funcs: sprintf

 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   UCRT docs: wide-to-multibyte conversion failure sets errno EILSEQ
 * note:  PROBE: the exact C-locale cutoff (wchar > 0xFF unrepresentable) is from memory; the 0xFF control case lives in pfs_probe_str_lc_ls_high_c_locale. Asserts only 0x0100.
 *        OBSERVED: %ls of an unencodable wchar_t returns -1/EILSEQ, but %lc returns 1 (only the literal 'a' is written) with errno EILSEQ.
 *        A (test bug) for lc_high (return value is 1, not -1); ls_high unchanged (passes).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_ls = "a%ls";
static const char *volatile f_lc = "a%lc";
static const wchar_t *volatile ws = L"\x0100";
static volatile wchar_t wc = 0x0100;
TC_CASE(ls_high)
{
    char b[16]; int n; errno = 0;
    n = sprintf(b, f_ls, ws);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(lc_high)
{
    char b[16]; int n; errno = 0;
    n = sprintf(b, f_lc, (wint_t)wc);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "a"); CHECK_EQ_I(errno, EILSEQ);
}
int main(void) { tc_begin(); TC_RUN(ls_high); TC_RUN(lc_high); return tc_end(); }
