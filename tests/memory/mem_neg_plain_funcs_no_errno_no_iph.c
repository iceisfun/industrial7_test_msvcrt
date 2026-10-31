/* TC
 * id:    mem_neg_plain_funcs_no_errno_no_iph
 * area:  memory
 * funcs: memcpy memmove memset memcmp memchr _memccpy _memicmp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   C11 7.24, 7.5 (errno use not documented for mem* functions); MS docs memcpy, memmove, memset, memcmp, memchr, _memccpy, _memicmp
 * note:  valid edge inputs (count 0, overlap, not found, mismatch) must not set errno nor call the invalid-parameter handler. On msvcrt.dll (no handler API) iph count is trivially 0.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>
#include <errno.h>

static volatile size_t v_n;
static volatile int v_c;

TC_CASE(zero_counts)
{
    char a[4] = "abc", b[4] = "xyz";
    errno = 4321; tc_iph_count = 0; v_n = 0;
    memcpy(a, b, v_n); memmove(a, b, v_n); memset(a, 'q', v_n);
    CHECK_EQ_I(memcmp(a, b, v_n), 0);
    CHECK(memchr(a, 'a', v_n) == NULL);
    CHECK(_memccpy(a, b, 'x', v_n) == NULL);
    CHECK_EQ_I(_memicmp(a, b, v_n), 0);
    CHECK_MEM(a, "abc", 4);
    CHECK_EQ_I(errno, 4321); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(overlap_and_notfound)
{
    char b[8] = "abcdefg", d[8];
    errno = 4321; tc_iph_count = 0; v_n = 5;
    memmove(b + 2, b, v_n);
    CHECK_MEM(b, "ababcde", 7);
    CHECK(memchr(b, 'z', v_n) == NULL);
    v_c = 'z';
    CHECK(_memccpy(d, b, v_c, v_n) == NULL);
    CHECK_EQ_I(errno, 4321); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(mismatch_and_set)
{
    char a[4] = "abc", b[4] = "abd", d[4];
    errno = 4321; tc_iph_count = 0; v_n = 3;
    CHECK(memcmp(a, b, v_n) < 0);
    CHECK(_memicmp(a, b, v_n) < 0);
    CHECK(_memicmp(b, a, v_n) > 0);
    memset(d, 'k', v_n); memcpy(d, a, v_n);
    CHECK_MEM(d, "abc", 3);
    CHECK_EQ_I(errno, 4321); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(zero_counts); TC_RUN(overlap_and_notfound); TC_RUN(mismatch_and_set);
    return tc_end();
}
