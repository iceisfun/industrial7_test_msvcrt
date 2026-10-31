/* TC
 * id:    pfw_neg_probe_wide_huge_width_overflow
 * area:  stdio_printf
 * funcs: _scwprintf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none (width/precision overflowing int)
 * note:  PROBE: "%2147483648d" and "%.4294967296d" (values not representable in int) make _scwprintf return < 0 without crashing, and set errno nonzero. Control "%5d" is standard (5).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile f_w = L"%2147483648d";
static const wchar_t *volatile f_p = L"%.4294967296d";
static const wchar_t *volatile f_c = L"%5d";

TC_CASE(control) { CHECK_EQ_I(_scwprintf(f_c, 1), 5); }
TC_CASE(huge_width) { int n; errno = 0; n = _scwprintf(f_w, 1); CHECK(n < 0); CHECK(errno != 0); }
TC_CASE(huge_precision) { int n; errno = 0; n = _scwprintf(f_p, 1); CHECK(n < 0); CHECK(errno != 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(control); TC_RUN(huge_width); TC_RUN(huge_precision);
    return tc_end();
}
