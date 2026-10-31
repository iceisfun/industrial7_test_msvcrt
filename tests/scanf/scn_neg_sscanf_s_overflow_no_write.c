/* TC
 * id:    scn_neg_sscanf_s_overflow_no_write
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: the size argument bounds the write; with a handler installed execution continues without overflow
 * note:  Buffer contents after failure (empty string) are probed in scn_neg_sscanf_s_too_small_ret_probe.
 *        The buffer ends exactly at a PAGE_NOACCESS page, so any write past size faults (test fails by crash).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(guard_page)
{
    DWORD old; int ok;
    char *base = (char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    char *buf; volatile char *v;
    CHECK(base != NULL); if (!base) return;
    ok = VirtualProtect(base + 4096, 4096, PAGE_NOACCESS, &old); CHECK(ok);
    buf = base + 4096 - 4;
    buf[0] = buf[1] = buf[2] = buf[3] = 'Q';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefghijklmnop";
    (void)sscanf_s((const char *)vin, "%s", buf, (unsigned)4);
    v = buf;
    CHECK(tc_iph_count >= 1);
    VirtualFree(base, 0, MEM_RELEASE);
}
TC_CASE(stack_sentinel)
{
    char a[16]; int i;
    for (i = 0; i < 16; i++) a[i] = 'S';
    CHECK(tc_iph_install()); vin = "abcdefghijklmnop";
    (void)sscanf_s((const char *)vin, "%s", a, (unsigned)4);
    for (i = 4; i < 16; i++) CHECK_EQ_I(a[i], 'S');
}
int main(void) { tc_begin(); TC_RUN(guard_page); TC_RUN(stack_sentinel); return tc_end(); }
