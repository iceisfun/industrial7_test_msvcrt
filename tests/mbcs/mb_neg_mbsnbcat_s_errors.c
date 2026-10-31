/* TC
 * id:    mb_neg_mbsnbcat_s_errors
 * area:  mbcs
 * funcs:
 * procs: _mbsnbcat_s _mbsnbcat_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsnbcat_s, _mbsncat_s, strncat_s error conditions (EINVAL / ERANGE, dst[0] set to 0 on ERANGE)
 * note:  count is in bytes. _TRUNCATE behaviour is in mb_neg_mbsnbcat_s_trunc_probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *ncat_t)(unsigned char *, size_t, const unsigned char *, size_t);
typedef int (__cdecl *ncat_l_t)(unsigned char *, size_t, const unsigned char *, size_t, void *);
static ncat_t p_ncat; static ncat_l_t p_ncat_l;
static const unsigned char * volatile np = NULL;
static unsigned char * volatile ndst = NULL;
static unsigned char sb[] = "cdef";
static const unsigned char * volatile src = sb;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT(code) do { CHECK_EQ_I(errno, code); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(overflow_erange)
{
    unsigned char d[4] = {'a','b',0};
    CHECK(p_ncat); if (!p_ncat) return;
    BEGIN; CHECK_EQ_I(p_ncat(d, 4, src, 4), ERANGE); EXPECT(ERANGE); CHECK_EQ_U(d[0], 0);
}
TC_CASE(l_overflow_erange)
{
    unsigned char e[4] = {'a','b',0};
    CHECK(p_ncat_l); if (!p_ncat_l) return;
    BEGIN; CHECK_EQ_I(p_ncat_l(e, 4, src, 4, NULL), ERANGE); EXPECT(ERANGE); CHECK_EQ_U(e[0], 0);
}
TC_CASE(count_zero_noop)
{
    unsigned char d[8] = {'a','b',0};
    CHECK(p_ncat); if (!p_ncat) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_ncat(d, 8, src, 0), 0); CHECK_STR((char *)d, "ab");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_src)
{
    unsigned char d[8] = {'a','b',0};
    CHECK(p_ncat); if (!p_ncat) return;
    BEGIN; CHECK_EQ_I(p_ncat(d, 8, np, 2), EINVAL); EXPECT(EINVAL);
}
TC_CASE(null_dst)
{
    CHECK(p_ncat); if (!p_ncat) return;
    BEGIN; CHECK_EQ_I(p_ncat(ndst, 8, src, 2), EINVAL); EXPECT(EINVAL);
}
TC_CASE(dst_unterminated)
{
    unsigned char d[4] = {'a','b','c','d'};
    CHECK(p_ncat); if (!p_ncat) return;
    BEGIN; CHECK_EQ_I(p_ncat(d, 4, src, 1), EINVAL); EXPECT(EINVAL);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncat = (ncat_t)tc_crt_proc("_mbsnbcat_s"); p_ncat_l = (ncat_l_t)tc_crt_proc("_mbsnbcat_s_l");
    TC_RUN(overflow_erange); TC_RUN(l_overflow_erange); TC_RUN(count_zero_noop);
    TC_RUN(null_src); TC_RUN(null_dst); TC_RUN(dst_unterminated);
    return tc_end();
}
