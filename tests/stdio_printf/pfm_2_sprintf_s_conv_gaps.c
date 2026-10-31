/* TC
 * id:    pfm_2_sprintf_s_conv_gaps
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s; printf type field characters
 * note:  %a/%A/%F are covered in pfm_2_sprintf_s_hexfloat_F_ucrt (not available before VS2013/ucrt). Wide variants: pfm_2_swprintf_s_conv_gaps.
 *        sprintf_s is a real export on older CRTs (mingw's libmsvcr80.a lacks it) so it is resolved at run time there; inline over
 *        __stdio_common_vsprintf_s on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
#define SPS sprintf_s
#else
typedef int (__cdecl *sps_t)(char *, size_t, const char *, ...);
static sps_t p_sps;
#define SPS p_sps
#endif
static volatile int vm42 = -42, v7 = 7, v8 = 8, v255 = 255;
static volatile unsigned vbig = 4000000000u;
static volatile wchar_t vwc = L'q';
static volatile double v15 = 1.5, v26 = 26.0;
static const wchar_t *volatile ws = L"wide";
static const char *volatile ns = "narrow";

TC_CASE(narrow_i_X_C)
{
    char b[64]; int r = SPS(b, sizeof b, "%i %X %C", vm42, v255, (int)vwc);
    CHECK_STR(b, "-42 FF q"); CHECK_EQ_I(r, 8);
}
TC_CASE(narrow_S_wide_string)
{
    char b[64]; int r = SPS(b, sizeof b, "%S|%s", ws, ns);
    CHECK_STR(b, "wide|narrow"); CHECK_EQ_I(r, 11);
}
TC_CASE(narrow_float_e_E_g_G)
{
    char b[96]; int r = SPS(b, sizeof b, "%.2e|%.1E|%g|%G", v15, v15, v26, v26);
#if TC_CRT_VER >= 140
    CHECK_STR(b, "1.50e+00|1.5E+00|26|26"); CHECK_EQ_I(r, 22);
#else
    CHECK_STR(b, "1.50e+000|1.5E+000|26|26"); CHECK_EQ_I(r, 24);
#endif
}
TC_CASE(narrow_exact_fit_ok)
{
    char b[8]; int r = SPS(b, 8, "%i|%X", v7, v255);
    CHECK_STR(b, "7|FF"); CHECK_EQ_I(r, 4);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_sps = (sps_t)tc_crt_proc("sprintf_s");
    if (!p_sps) { tc_write("TC FAIL " TC_TEST_ID "::__sprintf_s not exported\n"); return 2; }
#endif
    TC_RUN(narrow_i_X_C); TC_RUN(narrow_S_wide_string); TC_RUN(narrow_float_e_E_g_G); TC_RUN(narrow_exact_fit_ok);
    return tc_end();
}
