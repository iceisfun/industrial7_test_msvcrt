/* TC
 * id:    mb_neg_mbsnset_s_errors
 * area:  mbcs
 * funcs:
 * procs: _mbsnset_s _mbsnbset_s _mbsset_s _mbsset_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s, _mbsnset_s, _mbsset_s (dst NULL or size 0 -> EINVAL, handler invoked)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *nset_t)(unsigned char *, size_t, unsigned int, size_t);
typedef int (__cdecl *set_t)(unsigned char *, size_t, unsigned int);
typedef int (__cdecl *set_l_t)(unsigned char *, size_t, unsigned int, void *);
static nset_t p_nset, p_nbset; static set_t p_set; static set_l_t p_set_l;
static unsigned char * volatile ndst = NULL;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(null_dst_einval)
{
    CHECK(p_nset && p_nbset && p_set && p_set_l); if (!p_nset || !p_nbset || !p_set || !p_set_l) return;
    BEGIN; CHECK_EQ_I(p_nset(ndst, 8, 'x', 2), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_nbset(ndst, 8, 'x', 2), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_set(ndst, 8, 'x'), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_set_l(ndst, 8, 'x', NULL), EINVAL); EXPECT;
}
TC_CASE(size_zero_einval)
{
    unsigned char d[4] = {'a','b',0};
    CHECK(p_nset && p_nbset && p_set && p_set_l); if (!p_nset || !p_nbset || !p_set || !p_set_l) return;
    BEGIN; CHECK_EQ_I(p_nset(d, 0, 'x', 2), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_nbset(d, 0, 'x', 2), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_set(d, 0, 'x'), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_set_l(d, 0, 'x', NULL), EINVAL); EXPECT;
    CHECK_STR((char *)d, "ab");
}
TC_CASE(handler_counts)
{
    unsigned char d[4] = {'a','b',0};
    CHECK(p_nset && p_set); if (!p_nset || !p_set) return;
    tc_iph_count = 0;
    p_nset(ndst, 8, 'x', 1); CHECK(tc_iph_count >= 1);
    { int c1 = tc_iph_count; p_set(ndst, 8, 'x'); CHECK(tc_iph_count > c1); }
    { int c2 = tc_iph_count; CHECK_EQ_I(p_set(d, 4, 'x'), 0); CHECK_EQ_I(tc_iph_count, c2); }   /* success: no handler call */
    CHECK_STR((char *)d, "xx");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_nset = (nset_t)tc_crt_proc("_mbsnset_s"); p_nbset = (nset_t)tc_crt_proc("_mbsnbset_s");
    p_set = (set_t)tc_crt_proc("_mbsset_s"); p_set_l = (set_l_t)tc_crt_proc("_mbsset_s_l");
    TC_RUN(null_dst_einval); TC_RUN(size_zero_einval); TC_RUN(handler_counts);
    return tc_end();
}
