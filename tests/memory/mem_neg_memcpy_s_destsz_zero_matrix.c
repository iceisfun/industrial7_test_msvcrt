/* TC
 * id:    mem_neg_memcpy_s_destsz_zero_matrix
 * area:  memory
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s / memmove_s (error conditions table: destsz < count -> ERANGE)
 * note:  destsz == 0 with count > 0 and valid dest/src is ERANGE. Rows where two documented errors apply at once (precedence) live in mem_neg_memcpy_s_multi_fault_precedence_probe.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
static volatile size_t v_n = 4, v_z = 0;

static void rows(int (__cdecl *f)(void *, size_t, const void *, size_t))
{
    char d[8] = "GGGGGGG"; int r;
    tc_iph_count = 0; errno = 0;
    r = f(d, v_z, "abcd", v_n); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_MEM(d, "GGGGGGG", 8);
}
TC_CASE(memcpy_s_rows) { if (!tc_p_memcpy_s) TC_SKIP("missing"); rows(tc_p_memcpy_s); }
TC_CASE(memmove_s_rows) { if (!tc_p_memmove_s) TC_SKIP("missing"); rows(tc_p_memmove_s); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); CHECK(tc_iph_install());
    TC_RUN(memcpy_s_rows); TC_RUN(memmove_s_rows);
    return tc_end();
}
