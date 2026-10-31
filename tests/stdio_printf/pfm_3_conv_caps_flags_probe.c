/* TC
 * id:    pfm_3_conv_caps_flags_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf Type Field Characters (S = wide string in sprintf, narrow string in wprintf) and flag directives
 * note:  PROBE: the '+' and ' ' flags have no effect on %S (text unchanged); the '0' flag with a width on %S pads with '0' (Microsoft
 *        runtimes honour '0' for strings, unlike glibc where it is undefined).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char * volatile f_plus = "[%+S]";
static const char * volatile f_space = "[% S]";
static const char * volatile f_zero = "[%05S]";
static const char * volatile f_zeroleft = "[%-05S]";
static const wchar_t * volatile wf_plus = L"[%+S]";
static const wchar_t * volatile wf_space = L"[% S]";
static const wchar_t * volatile wf_zero = L"[%05S]";
static const wchar_t * const volatile ws = L"ab";
static const char * volatile ns = "ab";

TC_CASE(plus)  { char b[32]; sprintf(b, f_plus, ws);  CHECK_STR(b, "[ab]"); }
TC_CASE(space) { char b[32]; sprintf(b, f_space, ws); CHECK_STR(b, "[ab]"); }
TC_CASE(zero_width) { char b[32]; sprintf(b, f_zero, ws); CHECK_STR(b, "[000ab]"); }
TC_CASE(zero_left_width) { char b[32]; sprintf(b, f_zeroleft, ws); CHECK_STR(b, "[ab   ]"); }
TC_CASE(wide_plus)  { wchar_t b[32]; _snwprintf(b, 32, wf_plus, ns);  CHECK_WSTR(b, L"[ab]"); }
TC_CASE(wide_space) { wchar_t b[32]; _snwprintf(b, 32, wf_space, ns); CHECK_WSTR(b, L"[ab]"); }
TC_CASE(wide_zero_width) { wchar_t b[32]; _snwprintf(b, 32, wf_zero, ns); CHECK_WSTR(b, L"[000ab]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(plus); TC_RUN(space); TC_RUN(zero_width); TC_RUN(zero_left_width); TC_RUN(wide_plus); TC_RUN(wide_space); TC_RUN(wide_zero_width);
    return tc_end();
}
