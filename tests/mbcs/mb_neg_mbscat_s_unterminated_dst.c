/* TC
 * id:    mb_neg_mbscat_s_unterminated_dst
 * area:  mbcs
 * funcs:
 * procs: _mbscat_s _mbscat_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscat_s, _mbscat_s_l, strcat_s error conditions (EINVAL / ERANGE, dst[0] set to 0 on ERANGE)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
TC_DECL_PROC(int, _mbscat_s, (unsigned char *, size_t, const unsigned char *));
typedef int (__cdecl *cat_l_t)(unsigned char *, size_t, const unsigned char *, void *);
static cat_l_t p_cat_l;
static const unsigned char * volatile np = NULL;
static unsigned char * volatile ndst = NULL;
static unsigned char sb[] = "xy";
static const unsigned char * volatile src = sb;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT(code) do { CHECK_EQ_I(errno, code); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(dst_not_terminated_einval)
{
    unsigned char d[4] = {'a','b','c','d'};
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscat_s(d, 4, src), EINVAL); EXPECT(EINVAL);
}
TC_CASE(src_null_einval)
{
    unsigned char d[8] = {'a',0};
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscat_s(d, 8, np), EINVAL); EXPECT(EINVAL);
}
TC_CASE(dst_null_einval)
{
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscat_s(ndst, 8, src), EINVAL); EXPECT(EINVAL);
}
TC_CASE(size_zero_einval)
{
    unsigned char d[4] = {'a',0};
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscat_s(d, 0, src), EINVAL); EXPECT(EINVAL);
    CHECK_STR((char *)d, "a");
}
TC_CASE(l_variant_overflow_erange)
{
    unsigned char d[4] = {'a','b',0};
    if (!p_cat_l) { CHECK(p_cat_l != NULL); return; }
    BEGIN;
    CHECK_EQ_I(p_cat_l(d, 4, src, NULL), ERANGE);  /* "ab"+"xy"+NUL needs 5 */
    EXPECT(ERANGE);
}
TC_CASE(dst_cleared_on_erange)
{
    unsigned char d[4] = {'a','b',0};
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    BEGIN;
    CHECK_EQ_I(tc_p__mbscat_s(d, 4, src), ERANGE);
    EXPECT(ERANGE);
    CHECK_EQ_U(d[0], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_mbscat_s); p_cat_l = (cat_l_t)tc_crt_proc("_mbscat_s_l");
    TC_RUN(dst_not_terminated_einval); TC_RUN(src_null_einval); TC_RUN(dst_null_einval); TC_RUN(size_zero_einval);
    TC_RUN(l_variant_overflow_erange); TC_RUN(dst_cleared_on_erange);
    return tc_end();
}
