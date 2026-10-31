/* TC
 * id:    mem_neg_memcpy_s_handler_returns_value_pass_through
 * area:  memory
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s / memmove_s (return value; errno set to the same code; handler invoked per error)
 * note:  EINVAL for NULL dest, ERANGE for destsz < count. errno is zeroed before each call so it must be set by the failing call.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
static volatile size_t v_n = 4;

static void seq(int (__cdecl *f)(void *, size_t, const void *, size_t))
{
    char d[8]; int r;
    tc_iph_count = 0;
    errno = 0; r = f(NULL, 8, "abcd", v_n);  CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; r = f(d, 2, "abcd", v_n);     CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 2);
    errno = 0; r = f(d, 8, "abcd", v_n);     CHECK_EQ_I(r, 0);      CHECK_EQ_I(errno, 0);      CHECK_EQ_I(tc_iph_count, 2);
    CHECK_MEM(d, "abcd", 4);
    errno = 0; r = f(NULL, 8, "abcd", v_n);  CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 3);
}
TC_CASE(memcpy_s_seq) { if (!tc_p_memcpy_s) TC_SKIP("missing"); seq(tc_p_memcpy_s); }
TC_CASE(memmove_s_seq) { if (!tc_p_memmove_s) TC_SKIP("missing"); seq(tc_p_memmove_s); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); CHECK(tc_iph_install());
    TC_RUN(memcpy_s_seq); TC_RUN(memmove_s_seq);
    return tc_end();
}
