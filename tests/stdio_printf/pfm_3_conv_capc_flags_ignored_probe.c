/* TC
 * id:    pfm_3_conv_capc_flags_ignored_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf flag directives; C11 7.21.6.1p6 (+ and space are undefined for non-signed conversions in ISO C)
 * note:  PROBE: the '+', ' ' and '#' flags are silently ignored on %C (output is just the character), and the '0' flag with a width
 *        on %C pads with '0' (as it is known to do for %s on Windows) rather than spaces.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char * volatile f_plus = "[%+C]";
static const char * volatile f_space = "[% C]";
static const char * volatile f_hash = "[%#C]";
static const char * volatile f_zero5 = "[%05C]";
static const char * volatile f_zeroleft = "[%-05C]";
static const wchar_t * volatile wf_plus = L"[%+C]";
static const wchar_t * volatile wf_zero5 = L"[%05C]";

TC_CASE(plus)  { char b[32]; volatile wint_t c = L'A'; sprintf(b, f_plus, c);  CHECK_STR(b, "[A]"); }
TC_CASE(space) { char b[32]; volatile wint_t c = L'A'; sprintf(b, f_space, c); CHECK_STR(b, "[A]"); }
TC_CASE(hash)  { char b[32]; volatile wint_t c = L'A'; sprintf(b, f_hash, c);  CHECK_STR(b, "[A]"); }
TC_CASE(wide_plus) { wchar_t b[32]; volatile int c = 'A'; _snwprintf(b, 32, wf_plus, c); CHECK_WSTR(b, L"[A]"); }
TC_CASE(zero_width) { char b[32]; volatile wint_t c = L'A'; sprintf(b, f_zero5, c); CHECK_STR(b, "[0000A]"); }
TC_CASE(zero_left_width) { char b[32]; volatile wint_t c = L'A'; sprintf(b, f_zeroleft, c); CHECK_STR(b, "[A    ]"); }
TC_CASE(wide_zero_width) { wchar_t b[32]; volatile int c = 'A'; _snwprintf(b, 32, wf_zero5, c); CHECK_WSTR(b, L"[0000A]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(plus); TC_RUN(space); TC_RUN(hash); TC_RUN(wide_plus); TC_RUN(zero_width); TC_RUN(zero_left_width); TC_RUN(wide_zero_width);
    return tc_end();
}
