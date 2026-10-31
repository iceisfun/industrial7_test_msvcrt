/* TC
 * id:    mem_neg_memccpy_char_beyond_count
 * area:  memory
 * kind:  negative
 * funcs: _memccpy
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _memccpy: copies zero or more bytes of src to dest, stopping after c or after count bytes; returns NULL if c not among the first count bytes
 * note:  Must not see the character at index >= count, and must not write past dest+count.
 */
#include "tc.h"
#include <string.h>

static volatile size_t v_n;
static volatile int v_c;

TC_CASE(c_at_index_count_returns_null)
{
    char src[8] = "abcdXfgh", dst[8]; void *r;
    memset(dst, '.', 8); v_n = 4; v_c = 'X';
    r = _memccpy(dst, src, v_c, v_n);
    CHECK(r == NULL);
    CHECK_MEM(dst, "abcd....", 8);
}
TC_CASE(guard_after_count_untouched)
{
    char src[8] = "abXdefgh", dst[8]; void *r;
    memset(dst, 0x5a, 8); v_n = 2; v_c = 'X';
    r = _memccpy(dst, src, v_c, v_n);
    CHECK(r == NULL);
    CHECK_MEM(dst, "ab\x5a\x5a\x5a\x5a\x5a\x5a", 8);
}
TC_CASE(count_1_c_not_first_returns_null)
{
    char src[4] = "aXc", dst[4] = "...."; void *r;
    v_n = 1; v_c = 'X';
    r = _memccpy(dst, src, v_c, v_n);
    CHECK(r == NULL);
    CHECK_MEM(dst, "a...", 4);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_at_index_count_returns_null); TC_RUN(guard_after_count_untouched); TC_RUN(count_1_c_not_first_returns_null);
    return tc_end();
}
