/* TC
 * id:    wcs_neg_wcscpy_s_fill_ucrt
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _CrtSetDebugFillThreshold (debug CRT fills remaining buffer with 0xFE on failure)
 * note:  PROBE: release ucrtbase does NOT fill the remainder of dest after a failed wcscpy_s; elements after dest[0] keep their value.
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
    (void)r;   /* ERANGE / d[0]==0 are documented and asserted in wcs_neg_wcscpy_s_small; not mixed into this probe */
    for (i = 1; i < 8; i++) CHECK_EQ_I(d[i], 0x5A5A);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcscpy_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(no_fill_in_release);
    return tc_end();
}
