/* TC
 * id:    pfm_2_flag_plus_space_char_str_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf flags (+ and space documented for signed numbers only)
 * note:  PROBE: '+' and ' ' flags with %c/%s (undefined in C) are expected to be ignored by the MS CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static volatile int cx = 'x';
static const char *volatile sab = "ab";
static volatile wchar_t wx = L'x';

TC_CASE(plus_c) { char b[32]; sprintf(b, "%+c", cx); CHECK_STR(b, "x"); }
TC_CASE(space_s) { char b[32]; sprintf(b, "% s", sab); CHECK_STR(b, "ab"); }
TC_CASE(plus_s) { char b[32]; sprintf(b, "%+s", sab); CHECK_STR(b, "ab"); }
TC_CASE(wide_plus_c) { wchar_t b[32]; _snwprintf(b, 32, L"%+c", (int)wx); CHECK_WSTR(b, L"x"); }

int main(void) { tc_begin(); TC_RUN(plus_c); TC_RUN(space_s); TC_RUN(plus_s); TC_RUN(wide_plus_c); return tc_end(); }
