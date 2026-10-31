/* TC
 * id:    mem_memcpy_s_zero_count_null_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs memcpy_s (error table)
 * note:  PROBE: whether count==0 short-circuits before NULL validation in memcpy_s (docs table ambiguous). Handler installed; count recorded.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(count0_src_null_returns_0_no_handler)
{
    char d[4] = {1, 2, 3, 4}; volatile size_t z = 0;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_memcpy_s(d, 4, NULL, z), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(d[0], 1);
}
TC_CASE(count0_dest_null_destsz0_returns_0)
{
    char s[2] = {1, 2}; volatile size_t z = 0;
    if (!p_memcpy_s) return;
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_memcpy_s(NULL, z, s, z), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(count0_src_null_returns_0_no_handler); TC_RUN(count0_dest_null_destsz0_returns_0);
    return tc_end();
}
