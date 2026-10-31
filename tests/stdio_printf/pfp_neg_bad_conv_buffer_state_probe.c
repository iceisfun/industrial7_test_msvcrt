/* TC
 * id:    pfp_neg_bad_conv_buffer_state_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s: "buffer[0] is set to 0 on error"
 * note:  PROBE: state of the output buffer after sprintf_s fails on an invalid conversion ('ab%y') or a trailing '%'
 *        ('ab%'). Asserted: return -1, handler called, buf[0]==0, and bytes at index >= size untouched
 *        (size is smaller than the allocation, the tail is guard bytes). Contents of buf[1..size-1] are not asserted.
 */
#include "tc.h"
#include <stdio.h>

#define GUARD 0xCC
static void fill(unsigned char *b) { int i; for (i = 0; i < 48; i++) b[i] = GUARD; }
static int guard_ok(const unsigned char *b) { int i; for (i = 16; i < 48; i++) if (b[i] != GUARD) return 0; return 1; }

TC_CASE(probe_buf0_nul)
{
    unsigned char b[48]; int r; const char *volatile f = "ab%y";
    fill(b); tc_iph_count = 0;
    r = sprintf_s((char *)b, 16, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(probe_guard)
{
    unsigned char b[48]; const char *volatile f = "ab%y";
    fill(b); tc_iph_count = 0;
    sprintf_s((char *)b, 16, f);
    CHECK(guard_ok(b));
}
TC_CASE(probe_trailing_pct_buf0)
{
    unsigned char b[48]; int r; const char *volatile f = "ab%";
    fill(b); tc_iph_count = 0;
    r = sprintf_s((char *)b, 16, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); CHECK(guard_ok(b));
}

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(probe_buf0_nul); TC_RUN(probe_guard); TC_RUN(probe_trailing_pct_buf0);
    return tc_end();
}
