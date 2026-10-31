/* TC
 * id:    str_neg_strncpy_s_null_dest_zero_probe
 * area:  string
 * funcs:
 * procs: strncpy_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s (error table lists NULL dest -> EINVAL without a count==0 exception)
 * note:  PROBE: strncpy_s(NULL, 0, "abc", 0) is a special-cased "nothing to do" call in the UCRT (recollection of the source):
 *        returns 0, errno untouched, handler not called. Alternative on real Windows: EINVAL with one handler call.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));

TC_CASE(null_dest_size0_count0)
{
    volatile const char *s = "abc";
    errno = 0x5555; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(NULL, 0, (const char *)s, 0), 0);
    CHECK_EQ_I(errno, 0x5555);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(strncpy_s);
    if (!tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(null_dest_size0_count0);
    return tc_end();
}
