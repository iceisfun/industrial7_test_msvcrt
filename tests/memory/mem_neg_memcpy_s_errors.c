/* TC
 * id:    mem_neg_memcpy_s_errors
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s (error conditions table)
 * note:  Per the docs table: NULL dest -> EINVAL; NULL src -> EINVAL; destSize<count -> ERANGE. The invalid-parameter handler is invoked. Zero-fill of dest on failure is NOT asserted here (see the *_fill_probe / zeroing_probe tests).
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));

TC_CASE(null_dest_einval) { volatile size_t n = 4; if (!tc_p_memcpy_s) TC_SKIP("x"); tc_iph_count = 0; CHECK_EQ_I(tc_p_memcpy_s(NULL, 8, "abcd", n), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src_einval) { char d[4] = "abc"; volatile size_t n = 4; if (!tc_p_memcpy_s) TC_SKIP("x"); tc_iph_count = 0; CHECK_EQ_I(tc_p_memcpy_s(d, 4, NULL, n), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dest_too_small_erange) { char d[4] = "abc"; volatile size_t n = 8; if (!tc_p_memcpy_s) TC_SKIP("x"); tc_iph_count = 0; CHECK_EQ_I(tc_p_memcpy_s(d, 4, "12345678", n), ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(no_overrun_past_destsize) { char d[8]; int i; volatile size_t n = 6; if (!tc_p_memcpy_s) TC_SKIP("x"); for (i = 0; i < 8; i++) d[i] = 'Q'; CHECK(tc_p_memcpy_s(d, 4, "abcdef", n) != 0); CHECK_MEM(d + 4, "QQQQ", 4); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); CHECK(tc_iph_install());
    TC_RUN(null_dest_einval); TC_RUN(null_src_einval); TC_RUN(dest_too_small_erange); TC_RUN(no_overrun_past_destsize);
    return tc_end();
}
