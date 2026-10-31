/* TC
 * id:    mem_neg_memmove_s_params
 * area:  memory
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memmove_s
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(dest_null_EINVAL_handler)
{
    char s[4] = {1, 2, 3, 4}; volatile size_t n = 4;
    CHECK(p_memmove_s != NULL); if (!p_memmove_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memmove_s(NULL, 8, s, n), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(src_null_EINVAL_handler)
{
    char d[8]; volatile size_t n = 4;
    if (!p_memmove_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memmove_s(d, sizeof d, NULL, n), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(too_small_ERANGE_handler)
{
    char d[8] = {0}, s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    if (!p_memmove_s) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_memmove_s(d, 4, s, n), ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(guard_after_destsz_untouched)
{
    unsigned char d[8]; char s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    int i;
    if (!p_memmove_s) return;
    for (i = 0; i < 8; i++) d[i] = 0x5C;
    p_memmove_s(d, 4, s, n);
    for (i = 4; i < 8; i++) CHECK_EQ_I(d[i], 0x5C);
}
TC_CASE(errno_matches_return)
{
    char d[8] = {0}, s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    if (!p_memmove_s) return;
    errno = 0; p_memmove_s(NULL, 8, s, n); CHECK_EQ_I(errno, EINVAL);
    errno = 0; p_memmove_s(d, 8, NULL, n); CHECK_EQ_I(errno, EINVAL);
    errno = 0; p_memmove_s(d, 4, s, n);    CHECK_EQ_I(errno, ERANGE);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(dest_null_EINVAL_handler); TC_RUN(src_null_EINVAL_handler); TC_RUN(too_small_ERANGE_handler); TC_RUN(guard_after_destsz_untouched); TC_RUN(errno_matches_return);
    return tc_end();
}
