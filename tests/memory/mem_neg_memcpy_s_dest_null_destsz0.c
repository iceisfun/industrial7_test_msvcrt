/* TC
 * id:    mem_neg_memcpy_s_dest_null_destsz0
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s error table
 * note:  PROBE: memcpy_s(NULL,0,src,n>0): EINVAL (dest NULL) vs ERANGE (destsz<count) precedence. Believed NULL check comes first.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(returns_EINVAL_or_erange_recorded)
{
    char s[4] = {1, 2, 3, 4}; volatile size_t n = 4, z = 0;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(NULL, z, s, n), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(returns_EINVAL_or_erange_recorded);
    return tc_end();
}
