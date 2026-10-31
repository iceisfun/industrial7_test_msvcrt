/* TC
 * id:    pfm_2_flag_zero_string_char_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf flags; C11 7.21.6.1p6 (0 flag with s/c undefined)
 * note:  PROBE: 0 flag with %s/%c: MS CRT is believed to pad with zeros; '-' overrides '0' (documented for numbers).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static volatile int cx = 'x';
static const char *volatile sab = "ab";
static const wchar_t *volatile wab = L"ab";

TC_CASE(zero_s) { char b[32]; sprintf(b, "%05s", sab); CHECK_STR(b, "000ab"); }
TC_CASE(zero_c) { char b[32]; sprintf(b, "%05c", cx); CHECK_STR(b, "0000x"); }
TC_CASE(wide_zero_s) { wchar_t b[32]; _snwprintf(b, 32, L"%05s", wab); CHECK_WSTR(b, L"000ab"); }
TC_CASE(minus_zero_s) { char b[32]; sprintf(b, "%-05s", sab); CHECK_STR(b, "ab   "); }

int main(void) { tc_begin(); TC_RUN(zero_s); TC_RUN(zero_c); TC_RUN(wide_zero_s); TC_RUN(minus_zero_s); return tc_end(); }
