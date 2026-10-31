/* TC
 * id:    wcs_neg_wcscpy_s_src_unterminated_guard
 * area:  wstring
 * funcs:
 * procs: wcscpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcscpy_s (ERANGE when dest too small; dest[0] set to 0)
 * note:  (Despite the id, src IS terminated; the point is the too-small dest.) Probes that a too-small dest is never overrun (canary words after dest stay intact) when src is a long
 *        terminated string. The ucrt release build writes only inside dest; the dest[0]==0 part is documented.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
static const wchar_t *volatile vsrc = L"0123456789012345678901234567890123456789012345678901234567890123456789";
struct blk { wchar_t d[4]; wchar_t canary[8]; };

TC_CASE(canary_after_dest)
{
    struct blk b; int i, r;
    if (!tc_p_wcscpy_s || !tc_iph_install()) TC_SKIP("missing");
    for (i = 0; i < 8; i++) b.canary[i] = 0x5a5a;
    tc_iph_count = 0;
    r = tc_p_wcscpy_s(b.d, 4, vsrc);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 8; i++) CHECK_EQ_I(b.canary[i], 0x5a5a);
}
TC_CASE(dest0_cleared)
{
    struct blk b; int r;
    if (!tc_p_wcscpy_s || !tc_iph_install()) TC_SKIP("missing");
    b.d[0] = L'x'; tc_iph_count = 0;
    r = tc_p_wcscpy_s(b.d, 4, vsrc);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b.d[0], 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcscpy_s);
    TC_RUN(canary_after_dest); TC_RUN(dest0_cleared);
    return tc_end();
}
