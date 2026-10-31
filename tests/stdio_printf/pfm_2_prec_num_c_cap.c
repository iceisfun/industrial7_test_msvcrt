/* TC
 * id:    pfm_2_prec_num_c_cap
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf Precision Specification: precision has no effect when the type is c or C; width still pads
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int vc = 'x';
static volatile wchar_t vwc = L'x';

TC_CASE(narrow_c_prec) { char b[16]; int r = sprintf(b, "%.3c", vc); CHECK_STR(b, "x"); CHECK_EQ_I(r, 1); }
TC_CASE(narrow_c_width_prec) { char b[16]; int r = sprintf(b, "%5.3c", vc); CHECK_STR(b, "    x"); CHECK_EQ_I(r, 5); }
TC_CASE(narrow_C_prec) { char b[16]; int r = sprintf(b, "%.3C", (int)vwc); CHECK_STR(b, "x"); CHECK_EQ_I(r, 1); }
TC_CASE(narrow_c_prec_zero) { char b[16]; int r = sprintf(b, "%.0c|%.10c", vc, vc); CHECK_STR(b, "x|x"); CHECK_EQ_I(r, 3); }
TC_CASE(wide_c_prec) { wchar_t b[16]; int r = _snwprintf(b, 16, L"%.3c", (int)vwc); b[r > 0 ? r : 0] = 0; CHECK_WSTR(b, L"x"); CHECK_EQ_I(r, 1); }
TC_CASE(wide_c_width_left) { wchar_t b[16]; int r = _snwprintf(b, 16, L"%-4.2c|", (int)vwc); b[r > 0 ? r : 0] = 0; CHECK_WSTR(b, L"x   |"); CHECK_EQ_I(r, 5); }

int main(void)
{
    tc_begin();
    TC_RUN(narrow_c_prec); TC_RUN(narrow_c_width_prec); TC_RUN(narrow_C_prec); TC_RUN(narrow_c_prec_zero);
    TC_RUN(wide_c_prec); TC_RUN(wide_c_width_left);
    return tc_end();
}
