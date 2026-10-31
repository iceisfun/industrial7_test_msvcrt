/* TC
 * id:    mb_neg_mbscpy_overflow_unprotected
 * area:  mbcs
 * funcs: _mbsncpy _mbsncat _setmbcp _getmbcp
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbsncpy, _mbsncat; UCRT validates NULL arguments (invalid-parameter handler, EINVAL)
 * note:  The spec asked for _mbscpy/_mbscat, but neither is exported by ucrtbase or msvcr120 (only the _s forms), so
 *        only the n-variants are tested. The n-variants are legacy unbounded-destination functions; UCRT validates
 *        NULL dst/src (handler called, errno EINVAL, returns NULL). Count 0 returns dst untouched.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static int old_cp;
static unsigned char *volatile v_null;

TC_CASE(ncpy_null_dst)
{
    unsigned char src[] = "abc";
    unsigned char *r;
    tc_iph_count = 0; errno = 0;
    r = _mbsncpy(v_null, src, 2);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ncpy_null_src)
{
    unsigned char dst[8] = {'x', 'y', 0};
    unsigned char *r;
    tc_iph_count = 0; errno = 0;
    r = _mbsncpy(dst, v_null, 2);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_U(dst[0], 'x');
}
TC_CASE(ncat_null_dst)
{
    unsigned char src[] = "abc";
    unsigned char *r;
    tc_iph_count = 0; errno = 0;
    r = _mbsncat(v_null, src, 2);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ncat_null_src)
{
    unsigned char dst[8] = {'x', 'y', 0};
    unsigned char *r;
    tc_iph_count = 0; errno = 0;
    r = _mbsncat(dst, v_null, 2);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR((char *)dst, "xy");
}
TC_CASE(ncpy_count_zero_untouched)
{
    unsigned char dst[8] = {'x', 'y', 'z', 0}, src[] = "abc";
    volatile size_t n = 0;
    unsigned char *r = _mbsncpy(dst, src, n);
    CHECK(r == dst);
    CHECK_STR((char *)dst, "xyz");
}
TC_CASE(ncat_count_zero_untouched)
{
    unsigned char dst[8] = {'x', 'y', 'z', 0}, src[] = "abc";
    volatile size_t n = 0;
    unsigned char *r = _mbsncat(dst, src, n);
    CHECK(r == dst);
    CHECK_STR((char *)dst, "xyz");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(ncpy_null_dst); TC_RUN(ncpy_null_src); TC_RUN(ncat_null_dst); TC_RUN(ncat_null_src);
    TC_RUN(ncpy_count_zero_untouched); TC_RUN(ncat_count_zero_untouched);
    _setmbcp(old_cp);
    return tc_end();
}
