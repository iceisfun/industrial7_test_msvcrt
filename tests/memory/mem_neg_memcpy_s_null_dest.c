/* TC
 * id:    mem_neg_memcpy_s_null_dest
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s: dest NULL -> EINVAL
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(returns_EINVAL)
{
    char s[4] = {1, 2, 3, 4}; volatile size_t n = 4;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    CHECK_EQ_I(p_memcpy_s(NULL, 8, s, n), EINVAL);
}
TC_CASE(errno_EINVAL)
{
    char s[4] = {1, 2, 3, 4}; volatile size_t n = 4;
    if (!p_memcpy_s) return;
    errno = 0; p_memcpy_s(NULL, 8, s, n);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(handler_called_once)
{
    char s[4] = {1, 2, 3, 4}; volatile size_t n = 4;
    if (!p_memcpy_s) return;
    tc_iph_count = 0; p_memcpy_s(NULL, 8, s, n);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(src_unchanged)
{
    char s[4] = {1, 2, 3, 4}; const char w[4] = {1, 2, 3, 4}; volatile size_t n = 4;
    if (!p_memcpy_s) return;
    p_memcpy_s(NULL, 8, s, n);
    CHECK_MEM(s, w, 4);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(returns_EINVAL); TC_RUN(errno_EINVAL); TC_RUN(handler_called_once); TC_RUN(src_unchanged);
    return tc_end();
}
