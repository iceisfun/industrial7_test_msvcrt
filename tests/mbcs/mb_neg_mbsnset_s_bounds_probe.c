/* TC
 * id:    mb_neg_mbsnset_s_bounds_probe
 * area:  mbcs
 * funcs:
 * procs: _mbsnset_s _mbsnbset_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsnset_s (strnset_s semantics)
 * note:  PROBE: unterminated dst within size -> EINVAL; count 0 -> returns 0 and leaves dst unchanged; count beyond the string's
 *        NUL (but inside the buffer) stops at the NUL and returns 0 (no ERANGE); _TRUNCATE not asserted.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *nset_t)(unsigned char *, size_t, unsigned int, size_t);
static nset_t p_nset, p_nbset;
TC_CASE(unterminated_dst_einval)
{
    unsigned char d[3] = {'a','b','c'};
    CHECK(p_nset); if (!p_nset) return;
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_nset(d, 3, 'x', 2), EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(count_zero_returns_ok)
{
    unsigned char d[4] = {'a','b',0};
    CHECK(p_nset && p_nbset); if (!p_nset || !p_nbset) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_nset(d, 4, 'x', 0), 0); CHECK_STR((char *)d, "ab");
    CHECK_EQ_I(p_nbset(d, 4, 'x', 0), 0); CHECK_STR((char *)d, "ab");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_exceeds_buffer_stops_at_nul)
{
    unsigned char d[8] = {'a','b',0};
    CHECK(p_nset); if (!p_nset) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_nset(d, 8, 'x', 100), 0);
    CHECK_STR((char *)d, "xx");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_nset = (nset_t)tc_crt_proc("_mbsnset_s"); p_nbset = (nset_t)tc_crt_proc("_mbsnbset_s");
    TC_RUN(unterminated_dst_einval); TC_RUN(count_zero_returns_ok); TC_RUN(count_exceeds_buffer_stops_at_nul);
    return tc_end();
}
