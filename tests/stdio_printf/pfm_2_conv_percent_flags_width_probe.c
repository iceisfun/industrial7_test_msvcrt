/* TC
 * id:    pfm_2_conv_percent_flags_width_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (flags/width/precision with %% undefined)
 * note:  PROBE: Microsoft CRTs treat "%5%", "%-5%", "%.2%", "%05%" as a plain "%" (width/flags/precision ignored for the percent conversion).
 *        The alternative (padding to width) is the other plausible behaviour; real Windows decides.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[32]; static wchar_t w[32];
TC_CASE(w5)    { const char * volatile f = "%5%";  sprintf(b, f, 1); CHECK_STR(b, "%"); }
TC_CASE(m5)    { const char * volatile f = "%-5%"; sprintf(b, f, 1); CHECK_STR(b, "%"); }
TC_CASE(p2)    { const char * volatile f = "%.2%"; sprintf(b, f, 1); CHECK_STR(b, "%"); }
TC_CASE(z5)    { const char * volatile f = "%05%"; sprintf(b, f, 1); CHECK_STR(b, "%"); }
TC_CASE(wide5) { const wchar_t * volatile f = L"%5%"; _snwprintf(w, 31, f, 1); CHECK_WSTR(w, L"%"); }
TC_CASE(wide_m5) { const wchar_t * volatile f = L"%-5%"; _snwprintf(w, 31, f, 1); CHECK_WSTR(w, L"%"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(w5); TC_RUN(m5); TC_RUN(p2); TC_RUN(z5); TC_RUN(wide5); TC_RUN(wide_m5);
    return tc_end();
}
