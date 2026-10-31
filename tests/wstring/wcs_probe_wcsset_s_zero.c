/* TC
 * id:    wcs_probe_wcsset_s_zero
 * area:  wstring
 * funcs:
 * procs: _wcsset_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _wcsset_s (silent on a zero fill value)
 * note:  PROBE: _wcsset_s(buf, n, 0) on a terminated buffer. Expected: returns 0 and zeroes the characters before the original
 *        terminator (the loop is driven by the original NUL position), so buf[0..len-1] == 0.
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(errno_t, _wcsset_s, (wchar_t *, size_t, wchar_t));
static volatile size_t v_sz = 8;

TC_CASE(zero_fill)
{
    wchar_t b[8] = L"abc"; errno_t e;
    if (!tc_p__wcsset_s) TC_SKIP("missing");
    b[4] = L'Q';
    e = tc_p__wcsset_s(b, v_sz, 0);
    CHECK_EQ_I(e, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[4], L'Q');
}
int main(void) { tc_begin(); TC_LOAD_PROC(_wcsset_s); CHECK(tc_p__wcsset_s != NULL); TC_RUN(zero_fill); return tc_end(); }
