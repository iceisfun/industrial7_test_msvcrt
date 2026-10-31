/* TC
 * id:    pfm_2_neg_snprintf_s_errno_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   >=120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s (count >= buffer size without _TRUNCATE is an invalid parameter; truncation under _TRUNCATE)
 * note:  PROBES: (1) _snprintf_s(b,4,4,"%d",12345) (count == size, no _TRUNCATE, output too long) returns -1, calls the invalid parameter
 *        handler once, sets errno ERANGE and leaves b[0]==0; (2) with _TRUNCATE truncation sets errno ERANGE and does NOT call the handler.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(count_eq_size_overflow)
{
    char b[8]; const char * volatile f = "%d"; int n; b[0] = 'Q'; tc_iph_count = 0; errno = 0;
    n = _snprintf_s(b, 4, 4, f, 12345);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(truncate_errno_no_handler)
{
    char b[8]; const char * volatile f = "%d"; int n; tc_iph_count = 0; errno = 0;
    n = _snprintf_s(b, 4, _TRUNCATE, f, 12345);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, ERANGE); CHECK_STR(b, "123");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(count_eq_size_overflow); TC_RUN(truncate_errno_no_handler); return tc_end(); }
