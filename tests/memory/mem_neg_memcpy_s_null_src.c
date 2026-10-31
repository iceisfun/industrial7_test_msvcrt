/* TC
 * id:    mem_neg_memcpy_s_null_src
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s: src NULL -> EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(returns_EINVAL)
{
    char d[8]; volatile size_t n = 4;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    CHECK_EQ_I(p_memcpy_s(d, sizeof d, NULL, n), EINVAL);
}
TC_CASE(errno_EINVAL)
{
    char d[8]; volatile size_t n = 4;
    if (!p_memcpy_s) return;
    errno = 0; p_memcpy_s(d, sizeof d, NULL, n);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(handler_called_once)
{
    char d[8]; volatile size_t n = 4;
    if (!p_memcpy_s) return;
    tc_iph_count = 0; p_memcpy_s(d, sizeof d, NULL, n);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(returns_EINVAL); TC_RUN(errno_EINVAL); TC_RUN(handler_called_once);
    return tc_end();
}
