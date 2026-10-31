/* TC
 * id:    str_neg_strncpy_s_count_zero_cases
 * area:  string
 * funcs:
 * procs: strncpy_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s error table (NULL dest -> EINVAL, size 0 -> EINVAL, dest not modified); C11 K.3.7.2.4 (count 0 leaves dest[0] == 0)
 * note:  The special case strncpy_s(NULL, 0, s, 0) -> 0 is NOT asserted here (not in the documented table); see str_neg_strncpy_s_null_dest_zero_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));

TC_CASE(null_dest_size0_count3_einval)
{
    volatile const char *s = "abc";
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(NULL, 0, (const char *)s, 3), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(count0_valid_dest_empties)
{
    char b[8]; volatile const char *s = "abc";
    memset(b, 'X', sizeof b); tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(b, sizeof b, (const char *)s, 0), 0);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(dest_nonnull_size0_einval)
{
    char b[4]; volatile const char *s = "abc";
    memset(b, 'X', sizeof b); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(b, 0, (const char *)s, 0), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'X');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(strncpy_s);
    if (!tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(null_dest_size0_count3_einval);
    TC_RUN(count0_valid_dest_empties); TC_RUN(dest_nonnull_size0_einval);
    return tc_end();
}
