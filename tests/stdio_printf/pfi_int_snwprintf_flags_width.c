/* TC
 * id:    pfi_int_snwprintf_flags_width
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * note:  prec_overrides_zero asserted on ucrtbase only; older CRTs are probed separately.
 * ref:   C11 7.21.6.1 (flags, width, precision, '*'); MS docs printf width/precision specification
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(flags) { wchar_t b[64]; _snwprintf(b, 64, L"[%-5d][%+d][% d][%05d]", 42, 42, 42, 42); CHECK_WSTR(b, L"[42   ][+42][ 42][00042]"); }
TC_CASE(alt) { wchar_t b[64]; _snwprintf(b, 64, L"%#x %#o %#X", 255, 8, 255); CHECK_WSTR(b, L"0xff 010 0XFF"); }
TC_CASE(prec_zero) { wchar_t b[64]; volatile int z = 0; int n = _snwprintf(b, 64, L"[%.0d]", z); CHECK_WSTR(b, L"[]"); CHECK_EQ_I(n, 2); }
TC_CASE(prec_overrides_zero)
{
#ifdef TC_UCRT
    wchar_t b[64]; _snwprintf(b, 64, L"[%08.3d][%08.3d]", 7, -7); CHECK_WSTR(b, L"[     007][    -007]");
#else
    TC_SKIP("0 flag vs precision on Win7-era CRTs: see pfi_probe_snwprintf_prec_zero_flag_old");
#endif
}
TC_CASE(star_neg_width) { wchar_t b[64]; _snwprintf(b, 64, L"[%*d][%*d]", -5, 42, 5, 42); CHECK_WSTR(b, L"[42   ][   42]"); }
TC_CASE(star_neg_prec) { wchar_t b[64]; _snwprintf(b, 64, L"[%.*d]", -1, 42); CHECK_WSTR(b, L"[42]"); }
int main(void) { tc_begin(); TC_RUN(flags); TC_RUN(alt); TC_RUN(prec_zero); TC_RUN(prec_overrides_zero); TC_RUN(star_neg_width); TC_RUN(star_neg_prec); return tc_end(); }
