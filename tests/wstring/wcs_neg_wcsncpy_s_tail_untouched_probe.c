/* TC
 * id:    wcs_neg_wcsncpy_s_tail_untouched_probe
 * area:  wstring
 * funcs:
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcsncpy_s
 * note:  Probes that a successful wcsncpy_s(d, 8, src, 3) with unterminated src writes only count chars + NUL and leaves
 *        the rest of dest untouched (release CRTs; the debug CRT fills with 0xFE, not used here).
 */
#include "tc.h"
#include <wchar.h>

TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));

TC_CASE(dest_untouched_beyond_count_plus_nul)
{
    static const wchar_t src[3] = { L'A', L'B', L'C' };
    wchar_t d[8]; int r, i;
    if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing");
    for (i = 0; i < 8; i++) d[i] = 0x7777;
    r = tc_p_wcsncpy_s(d, 8, src, 3);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(d[3], 0);
    for (i = 4; i < 8; i++) CHECK_EQ_I(d[i], 0x7777);
}
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncpy_s); TC_RUN(dest_untouched_beyond_count_plus_nul); return tc_end(); }
