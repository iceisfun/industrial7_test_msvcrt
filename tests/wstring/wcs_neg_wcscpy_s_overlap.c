/* TC
 * id:    wcs_neg_wcscpy_s_overlap
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcscpy_s: "behavior is undefined if the source and destination strings overlap"
 * note:  PROBE: overlapping call must not crash or hang. Result is either an error (dest[0]==0) or a plain forward copy ("cdef").
 *        Neither the return value nor iph count is asserted.
 */
#include "tc.h"

TC_DECL_PROC(int, wcscpy_s, (wchar_t *, size_t, const wchar_t *));

TC_CASE(src_inside_dest)
{
    wchar_t d[8] = { L'a', L'b', L'c', L'd', L'e', L'f', 0, 0 };
    wchar_t * volatile dp = d; wchar_t * volatile sp = d + 2; int r;
    if (!tc_p_wcscpy_s) TC_SKIP("not exported");
    r = tc_p_wcscpy_s(dp, 8, sp);
    (void)r;
    CHECK(d[0] == 0 || tc_wcsequ(d, L"cdef"));
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcscpy_s);
    tc_iph_install();
    TC_RUN(src_inside_dest);
    return tc_end();
}
