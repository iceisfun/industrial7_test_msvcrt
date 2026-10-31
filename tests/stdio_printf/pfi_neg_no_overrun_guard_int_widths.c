/* TC
 * id:    pfi_neg_no_overrun_guard_int_widths
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snprintf_s sprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf (returns negative if more than count chars needed), sprintf_s (overflow: buffer[0]=0, ERANGE), _snprintf_s (_TRUNCATE: truncates, NUL-terminates, returns -1)
 */
#include "tc.h"
#include <stdio.h>

static void prep(unsigned char *b) { int i; for (i = 0; i < 24; i++) b[i] = 0x5a; }
static int guard_ok(const unsigned char *b) { int i; for (i = 8; i < 24; i++) if (b[i] != 0x5a) return 0; return 1; }

TC_CASE(snprintf_wide)
{
    unsigned char b[24]; volatile int v = 1; int r; prep(b);
    r = _snprintf((char *)b, 8, "%20d", v);
    CHECK_EQ_I(r, -1); CHECK(guard_ok(b));
}
TC_CASE(sprintf_s_prec)
{
    unsigned char b[24]; volatile int v = 1; int r; prep(b); tc_iph_count = 0;
    r = sprintf_s((char *)b, 8, "%.20d", v);
    CHECK_EQ_I(r, -1); CHECK(guard_ok(b)); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_s_truncate)
{
    unsigned char b[24]; volatile int v = 1; int r; prep(b); tc_iph_count = 0;
    r = _snprintf_s((char *)b, 8, _TRUNCATE, "%-20d|", v);
    CHECK_EQ_I(r, -1); CHECK(guard_ok(b)); CHECK_EQ_I(b[7], 0); CHECK_EQ_I(b[0], '1'); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(snprintf_wide); TC_RUN(sprintf_s_prec); TC_RUN(snprintf_s_truncate);
    return tc_end();
}
