/* TC
 * id:    mb_neg_mbsncpy_s_dst_state_after_error
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsncpy_s _mbsnbcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbsnbcpy_s, _mbsncpy_s (EINVAL / ERANGE tables); secure CRT convention: dst[0] = 0 on error
 * note:  Buffer-state-after-error is the common secure-CRT convention (documented for strcpy_s; observed for the mbs forms).
 *        count==0 behaviour is split out to mb_probe_mbsncpy_s_count_zero. mingw import libs for some CRTs lack the _s forms, hence procs.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

typedef int (__cdecl *cpy_s_t)(unsigned char *, size_t, const unsigned char *, size_t);
static cpy_s_t p_ncpy_s, p_nbcpy_s;
static int old_cp;
static unsigned char *volatile v_null;
#define HAVE (p_ncpy_s && p_nbcpy_s)

TC_CASE(erange_clears_dst0)
{
    unsigned char dst[4] = {0xCC, 0xCC, 0xCC, 0xCC}; const unsigned char src[] = "abcdefgh";
    int r;
    if (!HAVE) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = p_nbcpy_s(dst, sizeof dst, src, 8);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_U(dst[0], 0); CHECK_EQ_I(tc_iph_count, 1);
    dst[0] = 0xCC; tc_iph_count = 0;
    r = p_ncpy_s(dst, sizeof dst, src, 8);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_U(dst[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(einval_src_null_clears)
{
    unsigned char dst[4] = {'x', 'y', 'z', 0};
    int r;
    if (!HAVE) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = p_nbcpy_s(dst, sizeof dst, v_null, 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_U(dst[0], 0); CHECK_EQ_I(tc_iph_count, 1);
    dst[0] = 'x'; tc_iph_count = 0;
    r = p_ncpy_s(dst, sizeof dst, v_null, 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_U(dst[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(nb_count_gt_size)
{
    /* count far larger than the buffer, source shorter than buffer: only the source length matters */
    unsigned char dst[8]; const unsigned char src[] = "abc";
    if (!HAVE) TC_SKIP("not exported");
    CHECK_EQ_I(p_nbcpy_s(dst, sizeof dst, src, 100), 0);
    CHECK_STR((char *)dst, "abc");
    /* source longer than buffer, count > size and not _TRUNCATE: ERANGE */
    tc_iph_count = 0;
    CHECK_EQ_I(p_nbcpy_s(dst, sizeof dst, (const unsigned char *)"0123456789", 100), ERANGE);
    CHECK_EQ_U(dst[0], 0);
}
TC_CASE(size_zero)
{
    unsigned char dst[4] = {'x', 'y', 'z', 0}; const unsigned char src[] = "abc";
    if (!HAVE) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_nbcpy_s(dst, 0, src, 2), EINVAL);
    CHECK_EQ_I(p_ncpy_s(dst, 0, src, 2), EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
    CHECK_STR((char *)dst, "xyz");
}
TC_CASE(sentinel_bytes_beyond_untouched)
{
    unsigned char dst[12]; const unsigned char src[] = "ab"; int i;
    if (!HAVE) TC_SKIP("not exported");
    for (i = 0; i < 12; i++) dst[i] = 0xCC;
    CHECK_EQ_I(p_nbcpy_s(dst, sizeof dst, src, 2), 0);
    CHECK_STR((char *)dst, "ab");
    for (i = 3; i < 12; i++) CHECK_EQ_U(dst[i], 0xCC);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncpy_s = (cpy_s_t)tc_crt_proc("_mbsncpy_s"); p_nbcpy_s = (cpy_s_t)tc_crt_proc("_mbsnbcpy_s");
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(erange_clears_dst0); TC_RUN(einval_src_null_clears);
    TC_RUN(nb_count_gt_size); TC_RUN(size_zero); TC_RUN(sentinel_bytes_beyond_untouched);
    _setmbcp(old_cp);
    return tc_end();
}
