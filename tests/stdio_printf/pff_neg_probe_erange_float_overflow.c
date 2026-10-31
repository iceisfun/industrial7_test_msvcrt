/* TC
 * id:    pff_neg_probe_erange_float_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs sprintf_s / _snprintf_s error tables
 * note:  PROBES (a) errno==ERANGE after sprintf_s / _snprintf_s overflow with float output, (b) the exact digits UCRT prints for
 *        1e308 with %f (the exact binary value 1000000000000000010979063629440455417404923096773118463368106829...), i.e. the UCRT
 *        does not pad with zeros after 17 significant digits.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

TC_CASE(sprintf_s_erange)
{
    char b[400]; volatile double v = 1e308; int r;
    errno = 0; r = sprintf_s(b, 64, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(snprintf_s_erange)
{
    char b[8]; volatile double v = 3.14159; int r;
    errno = 0; r = _snprintf_s(b, 8, 100, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(exact_digits_1e308)
{
    char b[96]; volatile double v = 1e308; int r;
    memset(b, 'x', sizeof b);
    r = snprintf(b, 64, "%f", v);
    CHECK_EQ_I(r, 316);
    CHECK(strncmp(b, "1000000000000000010979063629440455417404923096773118463368106829", 62) == 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_erange); TC_RUN(snprintf_s_erange); TC_RUN(exact_digits_1e308);
    return tc_end();
}
