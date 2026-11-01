/* TC
 * id:    ucr_neg_stdio_vsprintf_wcs_eilseq
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf type field (%ls/%lc), wcrtomb (EILSEQ when a wide char cannot be converted)
 * note:  Win10 22H2 (observed, "C" locale): %ls of an unconvertible wide char (0x100) fails the call: -1 with EILSEQ. %lc of the same char sets EILSEQ but
 *        the call still succeeds, dropping the character (result "abyz", 4); deviates from C11 7.21.6.1p14 (an encoding error must give a negative return).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vs_t p_vs;
static int calls(char *b, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(ls_unconvertible)
{
    static wchar_t w[] = { 0x0100, 0 };
    char b[64]; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "ab%lsyz", (wchar_t *)w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(lc_unconvertible_dropped)
{
    char b[64]; int r; volatile unsigned int wc = 0x0100;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "ab%lcyz", (wint_t)wc);
    /* observed on Win10 22H2: unlike %ls (which returns -1), a failed %lc conversion sets EILSEQ but printing continues: the char is dropped and "abyz" (4) is produced */
    CHECK_EQ_I(r, 4); CHECK_STR(b, "abyz"); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    TC_RUN(ls_unconvertible); TC_RUN(lc_unconvertible_dropped);
    return tc_end();
}
