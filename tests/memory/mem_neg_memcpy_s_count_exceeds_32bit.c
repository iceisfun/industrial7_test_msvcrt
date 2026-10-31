/* TC
 * id:    mem_neg_memcpy_s_count_exceeds_32bit
 * area:  memory
 * funcs:
 * procs: memcpy_s memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s / memmove_s (ERANGE when numberOfElements < count; count is size_t)
 * note:  count = 0x100000004 with destsz 8 must be ERANGE; an implementation truncating count to 32 bits would see 4 and succeed. x64 only (size_t is 32-bit on x86).
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));

static volatile unsigned long long v_count = 0x100000004ull;

TC_CASE(memcpy_s_erange)
{
    unsigned char d[16], s[16]; int i, r; size_t n = (size_t)v_count;
    if (sizeof(size_t) < 8) TC_SKIP("x86");
    if (!tc_p_memcpy_s) TC_SKIP("memcpy_s missing");
    for (i = 0; i < 16; i++) { d[i] = 'D'; s[i] = 'S'; }
    tc_iph_count = 0; errno = 0;
    r = tc_p_memcpy_s(d, 8, s, n);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(!(d[0] == 'S' && d[1] == 'S' && d[2] == 'S' && d[3] == 'S'));
    CHECK_MEM(d + 8, "DDDDDDDD", 8);
}
TC_CASE(memmove_s_erange)
{
    unsigned char d[16], s[16]; int i, r; size_t n = (size_t)v_count;
    if (sizeof(size_t) < 8) TC_SKIP("x86");
    if (!tc_p_memmove_s) TC_SKIP("memmove_s missing");
    for (i = 0; i < 16; i++) { d[i] = 'D'; s[i] = 'S'; }
    tc_iph_count = 0; errno = 0;
    r = tc_p_memmove_s(d, 8, s, n);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    /* not a partial copy of s: either unchanged or zeroed */
    for (i = 0; i < 4; i++) CHECK(d[i] == 'D' || d[i] == 0);
    CHECK_MEM(d + 8, "DDDDDDDD", 8);
}
TC_CASE(exact_32bit_wrap_multiple)
{   /* count 0x100000000 (low 32 bits zero) must also be ERANGE, not a successful zero-length copy */
    unsigned char d[8], s[8]; size_t n; int r;
    if (sizeof(size_t) < 8) TC_SKIP("x86");
    if (!tc_p_memcpy_s) TC_SKIP("memcpy_s missing");
    n = (size_t)(v_count - 4);
    d[0] = 'D'; s[0] = 'S';
    tc_iph_count = 0;
    r = tc_p_memcpy_s(d, 8, s, n);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); CHECK(tc_iph_install());
    TC_RUN(memcpy_s_erange); TC_RUN(memmove_s_erange); TC_RUN(exact_32bit_wrap_multiple);
    return tc_end();
}
