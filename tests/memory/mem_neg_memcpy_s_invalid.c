/* TC
 * id:    mem_neg_memcpy_s_invalid
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs memcpy_s, wmemcpy_s; memmove_s (return value table: NULL dest/src -> EINVAL, destSize < count -> ERANGE; invalid parameter handler invoked)
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *cpy_s_t)(void *, size_t, const void *, size_t);
static cpy_s_t p_memcpy_s, p_memmove_s;
static volatile size_t v_n;

TC_CASE(exported) { CHECK(p_memcpy_s != NULL); CHECK(p_memmove_s != NULL); }
TC_CASE(ok_returns_zero)
{
    unsigned char d[8], s[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; int r;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); tc_iph_count = 0; v_n = 8;
    r = p_memcpy_s(d, 8, s, v_n); CHECK_EQ_I(r, 0); CHECK_MEM(d, s, 8); CHECK_EQ_I(tc_iph_count, 0);
    if (p_memmove_s) { r = p_memmove_s(d, 8, s, 0); CHECK_EQ_I(r, 0); }
}
TC_CASE(null_dest)
{
    unsigned char s[4] = { 1, 2, 3, 4 }; int r;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); tc_iph_count = 0; v_n = 4;
    r = p_memcpy_s(NULL, 8, s, v_n); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src)
{
    unsigned char d[4]; int r;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); tc_iph_count = 0; v_n = 4;
    r = p_memcpy_s(d, 4, NULL, v_n); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_too_small)
{
    unsigned char d[4], s[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; unsigned char guard[4] = { 9, 9, 9, 9 }; int r;
    if (!p_memcpy_s) TC_SKIP("memcpy_s missing");
    tc_iph_install(); tc_iph_count = 0; v_n = 8;
    r = p_memcpy_s(d, 4, s, v_n); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    (void)guard;
}
TC_CASE(memmove_s_errors)
{
    unsigned char d[4], s[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; int r;
    if (!p_memmove_s) TC_SKIP("memmove_s missing");
    tc_iph_install(); tc_iph_count = 0; v_n = 8;
    r = p_memmove_s(d, 4, s, v_n); CHECK_EQ_I(r, ERANGE);
    r = p_memmove_s(NULL, 4, s, 4); CHECK_EQ_I(r, EINVAL);
    r = p_memmove_s(d, 4, NULL, 4); CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin(); p_memcpy_s = (cpy_s_t)tc_crt_proc("memcpy_s"); p_memmove_s = (cpy_s_t)tc_crt_proc("memmove_s");
    TC_RUN(exported); TC_RUN(ok_returns_zero); TC_RUN(null_dest); TC_RUN(null_src); TC_RUN(dest_too_small); TC_RUN(memmove_s_errors);
    return tc_end();
}
