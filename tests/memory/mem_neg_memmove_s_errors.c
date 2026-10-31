/* TC
 * id:    mem_neg_memmove_s_errors
 * area:  memory
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memmove_s (error conditions table)
 * note:  NULL dest/src -> EINVAL; destSize<count -> ERANGE; handler invoked. Dest contents after failure not asserted except no write past destSize.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));

TC_CASE(null_dest_einval) { volatile size_t n = 4; if (!tc_p_memmove_s) TC_SKIP("x"); tc_iph_count = 0; CHECK_EQ_I(tc_p_memmove_s(NULL, 8, "abcd", n), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src_einval) { char d[4] = "abc"; volatile size_t n = 4; if (!tc_p_memmove_s) TC_SKIP("x"); tc_iph_count = 0; CHECK_EQ_I(tc_p_memmove_s(d, 4, NULL, n), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dest_too_small_erange) { char d[8]; int i; volatile size_t n = 6; if (!tc_p_memmove_s) TC_SKIP("x"); for (i = 0; i < 8; i++) d[i] = 'Q'; tc_iph_count = 0; CHECK_EQ_I(tc_p_memmove_s(d, 4, "abcdef", n), ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_MEM(d + 4, "QQQQ", 4); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(memmove_s); CHECK(tc_iph_install());
    TC_RUN(null_dest_einval); TC_RUN(null_src_einval); TC_RUN(dest_too_small_erange);
    return tc_end();
}
