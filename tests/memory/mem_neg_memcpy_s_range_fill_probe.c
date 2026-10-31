/* TC
 * id:    mem_neg_memcpy_s_range_fill_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s; CRT behaviour
 * note:  PROBE: on ERANGE failure memcpy_s zero-fills dest[0..destsz); with destsz==0 nothing is written.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(dest_zeroed_after_erange)
{
    unsigned char d[8]; const unsigned char z[4] = {0}; char s[8] = {1, 2, 3, 4, 5, 6, 7, 8}; volatile size_t n = 8;
    int i;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    for (i = 0; i < 8; i++) d[i] = 0xAA;
    CHECK_EQ_I(p_memcpy_s(d, 4, s, n), ERANGE);
    CHECK_MEM(d, z, 4);
    CHECK_EQ_I(d[4], 0xAA);
}
TC_CASE(dest_zeroed_for_destsz_zero_no_write)
{
    unsigned char d[4] = {0xAA, 0xAA, 0xAA, 0xAA}; char s[2] = {1, 2}; volatile size_t n = 2, z = 0;
    if (!p_memcpy_s) return;
    p_memcpy_s(d, z, s, n);
    CHECK_EQ_I(d[0], 0xAA); CHECK_EQ_I(d[3], 0xAA);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(dest_zeroed_after_erange); TC_RUN(dest_zeroed_for_destsz_zero_no_write);
    return tc_end();
}
