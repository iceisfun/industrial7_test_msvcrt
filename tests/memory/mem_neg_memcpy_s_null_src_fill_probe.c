/* TC
 * id:    mem_neg_memcpy_s_null_src_fill_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs memcpy_s (remarks: dest filled on failure is not stated); CRT behaviour
 * note:  PROBE: when src is NULL, memcpy_s zero-fills dest[0..destsz). Believed from CRT source, not in the docs table.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

typedef int (__cdecl *mcs_t)(void *, size_t, const void *, size_t);
static mcs_t p_memcpy_s, p_memmove_s;
#define SM ((size_t)-1)

TC_CASE(dest_zeroed_for_destsz_bytes)
{
    unsigned char d[10]; const unsigned char z[8] = {0}; volatile size_t n = 4;
    int i;
    CHECK(p_memcpy_s != NULL); if (!p_memcpy_s) return;
    for (i = 0; i < 10; i++) d[i] = 0xAA;
    CHECK_EQ_I(p_memcpy_s(d, 8, NULL, n), EINVAL);
    CHECK_MEM(d, z, 8);
    CHECK_EQ_I(d[8], 0xAA); CHECK_EQ_I(d[9], 0xAA);
}
int main(void)
{
    tc_begin();
    p_memcpy_s = (mcs_t)tc_crt_proc("memcpy_s");
    p_memmove_s = (mcs_t)tc_crt_proc("memmove_s");
    tc_iph_install();
    TC_RUN(dest_zeroed_for_destsz_bytes);
    return tc_end();
}
