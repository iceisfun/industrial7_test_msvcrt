/* TC
 * id:    wcs_neg_wcscpy_s_fill_ucrt
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _CrtSetDebugFillThreshold (debug CRT fills remaining buffer with 0xFE on failure)
 * note:  PROBE: release ucrtbase does NOT fill the remainder of dest after a failed wcscpy_s; elements after dest[0] keep their value.
 *        Observed Win10 22H2 ucrtbase: wcscpy_s(d, 8, L"abcdefgh") returns ERANGE, d[0] = 0, and d[1..7] hold the SOURCE characters
 *        'b'..'h' (copied before the overflow was detected), i.e. they are neither the old 0x5A5A nor a debug fill pattern. The test
 *        asserts the absence of a fill pattern (not 0xFEFE/0xFDFD) and the observed copied characters. Stable.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, wcscpy_s, (wchar_t *, size_t, const wchar_t *));

TC_CASE(no_fill_in_release)
{
    wchar_t d[8]; int i, r; const wchar_t * volatile s = L"abcdefgh";
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) d[i] = 0x5A5A;
    r = tc_p_wcscpy_s(d, 8, s);   /* needs 9 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0);
    for (i = 1; i < 8; i++) { CHECK(d[i] != 0xFEFE && d[i] != 0xFDFD); CHECK_EQ_I(d[i], s[i]); }   /* observed: source chars, no fill */
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcscpy_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(no_fill_in_release);
    return tc_end();
}
