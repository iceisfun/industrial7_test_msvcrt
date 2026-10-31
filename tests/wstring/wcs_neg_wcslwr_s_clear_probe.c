/* TC
 * id:    wcs_neg_wcslwr_s_clear_probe
 * area:  wstring
 * funcs:
 * procs: _wcslwr_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcslwr_s
 * note:  PROBE: does ucrtbase _wcslwr_s set str[0]=0 when the buffer is too small (unterminated within size)?
 *        Docs do not say. Recollection of the UCRT source pattern (_RESET_STRING before _RETURN_DEST_NOT_NULL_TERMINATED)
 *        says it DOES clear. Asserts buffer[0]==0. Older CRTs not asserted (unknown).
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _wcslwr_s, (wchar_t *, size_t));

TC_CASE(buffer_state_after_small)
{
    wchar_t b[8] = { L'A', L'B', L'C', 0 };
    int r;
    if (!tc_p__wcslwr_s) TC_SKIP("not exported");
    r = tc_p__wcslwr_s(b, 3);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcslwr_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(buffer_state_after_small);
    return tc_end();
}
