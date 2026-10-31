/* TC
 * id:    pfm_2_swprintf_s_conv_gaps
 * area:  stdio_printf
 * funcs:
 * procs: swprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s; printf type field characters
 * note:  swprintf_s is an inline over __stdio_common_vswprintf_s on ucrtbase; on older CRTs it is called through the export
 *        (mingw's libmsvcr80.a lacks it). %F/%a variants: pfm_2_sprintf_s_hexfloat_F_ucrt.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
#define SWP swprintf_s
#else
typedef int (__cdecl *swp_t)(wchar_t *, size_t, const wchar_t *, ...);
static swp_t p_swp;
#define SWP p_swp
#endif
static volatile int vm42 = -42, v7 = 7, v8 = 8, v255 = 255;
static volatile unsigned vbig = 4000000000u;
static volatile wchar_t vwc = L'q';
static volatile double v15 = 1.5, v26 = 26.0;
static const wchar_t *volatile ws = L"wide";
static const char *volatile ns = "narrow";

TC_CASE(wide_i_u_o_x_X)
{
    wchar_t b[64]; int r = SWP(b, 64, L"%i %u %o %x %X", vm42, vbig, v8, v255, v255);
    CHECK_WSTR(b, L"-42 4000000000 10 ff FF"); CHECK_EQ_I(r, 23);
}
TC_CASE(wide_e_E_g_G)
{
    wchar_t b[96]; int r = SWP(b, 96, L"%.2e|%.1E|%g|%G", v15, v15, v26, v26);
#if TC_CRT_VER >= 140
    CHECK_WSTR(b, L"1.50e+00|1.5E+00|26|26"); CHECK_EQ_I(r, 22);
#else
    CHECK_WSTR(b, L"1.50e+000|1.5E+000|26|26"); CHECK_EQ_I(r, 24);
#endif
}
TC_CASE(wide_c_C_s_S)
{
    wchar_t b[64]; int r = SWP(b, 64, L"%c%C%s%S", (int)vwc, 'z', ws, ns);
    CHECK_WSTR(b, L"qzwidenarrow"); CHECK_EQ_I(r, 12);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_swp = (swp_t)tc_crt_proc("swprintf_s");
    if (!p_swp) { tc_write("TC FAIL " TC_TEST_ID "::__swprintf_s not exported\n"); return 2; }
#endif
    TC_RUN(wide_i_u_o_x_X); TC_RUN(wide_e_E_g_G); TC_RUN(wide_c_C_s_S);
    return tc_end();
}
