/* TC
 * id:    mb_neg_mbscpy_s_overlap_bounds
 * area:  mbcs
 * funcs:
 * procs: _mbscpy_s _mbscpy_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscpy_s, _mbscpy_s_l (error conditions table: EINVAL / ERANGE, dst[0] set to 0)
 * note:  despite the id, no overlapping-buffer case is tested (overlap is undefined per the docs); this covers bounds/NULL errors.
 *        procs because mingw's old import libs lack the _s functions.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
TC_DECL_PROC(int, _mbscpy_s, (unsigned char *, size_t, const unsigned char *));
typedef int (__cdecl *cpy_l_t)(unsigned char *, size_t, const unsigned char *, void *);
static cpy_l_t p_cpy_l;
static const unsigned char * volatile np = NULL;
static unsigned char * volatile ndst = NULL;
static unsigned char srcb[] = "abcd";
static const unsigned char * volatile src = srcb;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)

TC_CASE(size_zero_nonnull_dst_einval)
{
    unsigned char d[4] = {'x','y','z',0};
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscpy_s(d, 0, src), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK_STR((char *)d, "xyz");
}
TC_CASE(src_null_clears_dst0)
{
    unsigned char d[4] = {'x','y','z',0};
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscpy_s(d, 4, np), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    CHECK_EQ_U(d[0], 0);
}
TC_CASE(dst_null_size_zero)
{
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscpy_s(ndst, 0, src), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(dst_null_size_nonzero)
{
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscpy_s(ndst, 8, src), EINVAL);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(l_variant_overflow_erange)
{
    unsigned char d[3] = {'x','y',0};
    if (!p_cpy_l) { CHECK(p_cpy_l != NULL); return; }
    BEGIN;
    CHECK_EQ_I(p_cpy_l(d, 3, src, NULL), ERANGE);   /* "abcd" needs 5 bytes */
    CHECK_EQ_I(errno, ERANGE); CHECK(tc_iph_count >= 1);
    CHECK_EQ_U(d[0], 0);
}
TC_CASE(returned_value_is_errno_code)
{
    unsigned char d[2];
    int r;
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    BEGIN;
    r = tc_p__mbscpy_s(d, 2, src);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(r, errno);
    BEGIN;
    r = tc_p__mbscpy_s(d, 2, (const unsigned char *)"a");
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR((char *)d, "a");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_mbscpy_s); p_cpy_l = (cpy_l_t)tc_crt_proc("_mbscpy_s_l");
    TC_RUN(size_zero_nonnull_dst_einval); TC_RUN(src_null_clears_dst0); TC_RUN(dst_null_size_zero);
    TC_RUN(dst_null_size_nonzero); TC_RUN(l_variant_overflow_erange); TC_RUN(returned_value_is_errno_code);
    return tc_end();
}
