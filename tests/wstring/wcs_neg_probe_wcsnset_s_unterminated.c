/* TC
 * id:    wcs_neg_probe_wcsnset_s_unterminated
 * area:  wstring
 * funcs:
 * procs: _wcsnset_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcsnset_s (error table lists "not null-terminated within size" -> EINVAL)
 * note:  PROBE: _wcsnset_s(buf, 4, 'z', 2) on a buffer {a,b,c,d} with no NUL. The docs table says EINVAL, but an implementation
 *        that stops filling when count is exhausted might return 0 without scanning the whole buffer. Expected: EINVAL,
 *        errno EINVAL, one invalid-parameter call.
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(errno_t, _wcsnset_s, (wchar_t *, size_t, wchar_t, size_t));
static volatile size_t v_sz = 4;

TC_CASE(nset_unterminated)
{
    wchar_t b[4] = { L'a', L'b', L'c', L'd' }; errno_t e;
    if (!tc_p__wcsnset_s) TC_SKIP("missing");
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    e = tc_p__wcsnset_s(b, v_sz, L'z', 2);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_wcsnset_s); CHECK(tc_p__wcsnset_s != NULL); TC_RUN(nset_unterminated); return tc_end(); }
