/* TC
 * id:    mem_neg_memcpy_s_dest_too_small
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

TC_CASE(returns_ERANGE)
{
    char d[8] = {0}, s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    CHECK_EQ_I(p_memcpy_s(d, 4, s, n), ERANGE);
}
TC_CASE(errno_ERANGE)
{
    char d[8] = {0}, s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    if (!p_memcpy_s) return;
    errno = 0; p_memcpy_s(d, 4, s, n);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(handler_called_once)
{
    char d[8] = {0}, s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    if (!p_memcpy_s) return;
    tc_iph_count = 0; p_memcpy_s(d, 4, s, n);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(guard_bytes_after_destsz_untouched)
{
    unsigned char d[8]; char s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    int i;
    if (!p_memcpy_s) return;
    for (i = 0; i < 8; i++) d[i] = 0x5C;
    p_memcpy_s(d, 4, s, n);
    for (i = 4; i < 8; i++) CHECK_EQ_I(d[i], 0x5C);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(returns_ERANGE); TC_RUN(errno_ERANGE); TC_RUN(handler_called_once); TC_RUN(guard_bytes_after_destsz_untouched);
    return tc_end();
}
