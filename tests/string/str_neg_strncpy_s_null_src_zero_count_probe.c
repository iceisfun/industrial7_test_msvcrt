/* TC
 * id:    str_neg_strncpy_s_null_src_zero_count_probe
 * area:  string
 * funcs:
 * procs: strncpy_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s (src NULL -> EINVAL in the error table; count==0 interplay not stated)
 * note:  PROBE: strncpy_s(buf, 8, NULL, 0) with count 0 and src NULL. Guess (from recollection of the UCRT source order: dest/size validated, then count==0 clears dest and returns 0, then src validated): returns 0, no handler call, buf[0] == 0. Alternative on real Windows: EINVAL with handler called once.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));

TC_CASE(null_src_count0)
{
    char b[8]; const char *volatile s = NULL;
    memset(b, 'X', sizeof b); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(tc_p_strncpy_s(b, sizeof b, s, 0), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); TC_LOAD_PROC(strncpy_s);
    if (!tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(null_src_count0);
    return tc_end();
}
