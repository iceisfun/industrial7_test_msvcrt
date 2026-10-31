/* TC
 * id:    mem_neg_memmove_s_no_fill_probe
 * area:  memory
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memmove_s
 * note:  PROBE: memmove_s leaves dest UNCHANGED on error (unlike memcpy_s which zero-fills); believed from CRT source.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(src_null_dest_unchanged_or_zero)
{
    unsigned char d[8]; const unsigned char w[8] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA}; volatile size_t n = 4;
    int i;
    CHECK(p_memmove_s != NULL); if (!p_memmove_s) return;
    for (i = 0; i < 8; i++) d[i] = 0xAA;
    CHECK_EQ_I(p_memmove_s(d, 8, NULL, n), EINVAL);
    CHECK_MEM(d, w, 8);
}
TC_CASE(erange_dest_unchanged_or_zero)
{
    unsigned char d[8]; const unsigned char w[8] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA}; char s[8] = {1,2,3,4,5,6,7,8};
    volatile size_t n = 8; int i;
    if (!p_memmove_s) return;
    for (i = 0; i < 8; i++) d[i] = 0xAA;
    CHECK_EQ_I(p_memmove_s(d, 4, s, n), ERANGE);
    CHECK_MEM(d, w, 8);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(src_null_dest_unchanged_or_zero); TC_RUN(erange_dest_unchanged_or_zero);
    return tc_end();
}
