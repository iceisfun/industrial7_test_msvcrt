/* TC
 * id:    mem_neg_memcpy_s_huge_count
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s: destSize < count -> ERANGE
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(count_size_max)
{
    char d[8], s[8] = {0}; volatile size_t n = SM;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(d, sizeof d, s, n), ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(count_size_max_minus_1)
{
    char d[8], s[8] = {0}; volatile size_t n = SM - 1;
    if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(d, sizeof d, s, n), ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(count_destsz_plus_1)
{
    char d[8], s[16] = {0}; volatile size_t n = 9;
    if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(d, sizeof d, s, n), ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(count_size_max); TC_RUN(count_size_max_minus_1); TC_RUN(count_destsz_plus_1);
    return tc_end();
}
