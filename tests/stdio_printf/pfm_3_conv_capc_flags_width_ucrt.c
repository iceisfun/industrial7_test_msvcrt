/* TC
 * id:    pfm_3_conv_capc_flags_width_ucrt
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Type Field Characters (C = wide character in sprintf, narrow character in wprintf); width and '-' flag specification
 * note:  Width and left-justify only. The '+', ' ', '#' and '0' flags on %C are in pfm_3_conv_capc_flags_ignored_probe.
 *        _snwprintf stands in for swprintf (swprintf's signature differs across CRT generations).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char * volatile f_left = "[%-5C]";
static const char * volatile f_right = "[%5C]";
static const char * volatile f_plain = "[%1C]";
static const wchar_t * volatile wf_left = L"[%-5C]";
static const wchar_t * volatile wf_right = L"[%5C]";

TC_CASE(sprintf_left)  { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_left, c);  CHECK_STR(b, "[A    ]"); CHECK_EQ_I(r, 7); }
TC_CASE(sprintf_right) { char b[32]; volatile wint_t c = L'A'; int r = sprintf(b, f_right, c); CHECK_STR(b, "[    A]"); CHECK_EQ_I(r, 7); }
TC_CASE(sprintf_width_smaller) { char b[32]; volatile wint_t c = L'Q'; int r = sprintf(b, f_plain, c); CHECK_STR(b, "[Q]"); CHECK_EQ_I(r, 3); }
TC_CASE(swprintf_left)  { wchar_t b[32]; volatile int c = 'A'; int r = _snwprintf(b, 32, wf_left, c);  CHECK_WSTR(b, L"[A    ]"); CHECK_EQ_I(r, 7); }
TC_CASE(swprintf_right) { wchar_t b[32]; volatile int c = 'A'; int r = _snwprintf(b, 32, wf_right, c); CHECK_WSTR(b, L"[    A]"); CHECK_EQ_I(r, 7); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(sprintf_left); TC_RUN(sprintf_right); TC_RUN(sprintf_width_smaller); TC_RUN(swprintf_left); TC_RUN(swprintf_right);
    return tc_end();
}
