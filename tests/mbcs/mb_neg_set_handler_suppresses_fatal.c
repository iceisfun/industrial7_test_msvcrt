/* TC
 * id:    mb_neg_set_handler_suppresses_fatal
 * area:  mbcs
 * kind:  negative
 * funcs:
 * procs: _mbscat_s _mbscpy_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbscat_s, _set_invalid_parameter_handler
 * note:  With a handler installed the bad calls that are fatal by default return an error and the process continues.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *mbscat_s_t)(unsigned char *, size_t, const unsigned char *);
typedef int (__cdecl *mbscpy_s_t)(unsigned char *, size_t, const unsigned char *);
static mbscat_s_t p_cat; static mbscpy_s_t p_cpy;

TC_CASE(cat_overflow)
{
    unsigned char buf[6]; static const unsigned char *volatile a = (const unsigned char *)"abc";
    static const unsigned char *volatile b = (const unsigned char *)"defgh";
    CHECK_EQ_I(p_cpy(buf, 6, a), 0);
    tc_iph_count = 0;
    CHECK_EQ_I(p_cat(buf, 6, b), ERANGE);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(cpy_null_dest)
{
    static unsigned char *volatile nul = NULL; static const unsigned char *volatile a = (const unsigned char *)"abc";
    tc_iph_count = 0;
    CHECK_EQ_I(p_cpy(nul, 4, a), EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(cat_ok_no_handler_call)
{
    unsigned char buf[16]; static const unsigned char *volatile a = (const unsigned char *)"ab";
    CHECK_EQ_I(p_cpy(buf, 16, a), 0);
    tc_iph_count = 0;
    CHECK_EQ_I(p_cat(buf, 16, a), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR((const char *)buf, "abab");
}
int main(void)
{
    tc_begin();
    p_cat = (mbscat_s_t)tc_crt_proc("_mbscat_s"); p_cpy = (mbscpy_s_t)tc_crt_proc("_mbscpy_s");
    CHECK(p_cat && p_cpy);
    if (!p_cat || !p_cpy || !tc_iph_install()) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(cat_overflow); TC_RUN(cpy_null_dest); TC_RUN(cat_ok_no_handler_call);
    return tc_end();
}
