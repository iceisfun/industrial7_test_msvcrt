/* TC
 * id:    pfm_2_stdio_common_vsprintf_options_exp3_probe
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   Windows SDK corecrt_stdio_config.h: _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS (0x10); MS docs _set_output_format
 * note:  PROBES: Option 0x10 makes %e/%g print three-digit exponents like the pre-VS2015 CRTs; without it ucrt prints at least two digits.
 *        whether ucrtbase honours 0x10 in
 *        __stdio_common_vsprintf is the thing probed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int sv(unsigned __int64 opt, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(opt, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile double v15 = 1.5, big = 1e100;

TC_CASE(opt10_big)    { char b[64]; int r = sv(0x10, b, 64, "%e", big); CHECK_STR(b, "1.000000e+100"); CHECK_EQ_I(r, 13); }
TC_CASE(opt10_small)  { char b[64]; int r = sv(0x10, b, 64, "%e", v15); CHECK_STR(b, "1.500000e+000"); CHECK_EQ_I(r, 13); }
TC_CASE(opt10_E_g)    { char b[64]; int r = sv(0x10, b, 64, "%E|%.2g", v15 * 1e10, v15 * 1e10); CHECK_STR(b, "1.500000E+010|1.5e+010"); CHECK_EQ_I(r, 22); }
TC_CASE(opt10_f_unchanged) { char b[64]; int r = sv(0x10, b, 64, "%f", v15); CHECK_STR(b, "1.500000"); CHECK_EQ_I(r, 8); }

int main(void)
{
    tc_begin();
    TC_RUN(opt10_big);  TC_RUN(opt10_small);  TC_RUN(opt10_E_g); TC_RUN(opt10_f_unchanged);
    return tc_end();
}
