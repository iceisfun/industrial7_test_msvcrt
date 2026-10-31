/* TC
 * id:    mb_neg_mbs_s_functions_present_new_crts
 * area:  mbcs
 * funcs:
 * procs: _mbscat_s _mbsnbcat_s _mbsset_s _mbstok_s _mbslwr_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscat_s, _mbsnbcat_s, _mbsset_s, _mbstok_s, _mbslwr_s (Requirements: msvcr80 and later)
 * note:  Counterpart of mb_neg_mbs_functions_missing_on_old_crts. Also checks that a NULL destination
 *        is rejected with EINVAL through the invalid-parameter handler.
 */
#include "tc.h"
#include <errno.h>
typedef int (__cdecl *cat_s_t)(unsigned char *, size_t, const unsigned char *);

TC_CASE(resolve)
{
    CHECK(tc_crt_proc("_mbscat_s") != NULL);
    CHECK(tc_crt_proc("_mbsnbcat_s") != NULL);
    CHECK(tc_crt_proc("_mbsset_s") != NULL);
    CHECK(tc_crt_proc("_mbstok_s") != NULL);
    CHECK(tc_crt_proc("_mbslwr_s") != NULL);
}
TC_CASE(null_dest_rejected)
{
    cat_s_t f = (cat_s_t)tc_crt_proc("_mbscat_s");
    static const unsigned char src[] = "a";
    int r;
    if (!f) TC_SKIP("not exported");
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    r = f(NULL, 10, src);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(resolve); TC_RUN(null_dest_rejected);
    return tc_end();
}
