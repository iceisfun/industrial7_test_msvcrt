/* TC
 * id:    pfm_2_flag_hash_char_str_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf flags (# documented for o x X a e f g only)
 * note:  PROBE: '#' flag with %c %s %S is expected to be ignored (undefined in C).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static volatile int cx = 'x';
static const char *volatile sab = "ab";

TC_CASE(hash_c) { char b[32]; sprintf(b, "%#c", cx); CHECK_STR(b, "x"); }
TC_CASE(hash_s) { char b[32]; sprintf(b, "%#s", sab); CHECK_STR(b, "ab"); }
TC_CASE(hash_s_width) { char b[32]; sprintf(b, "%#5s", sab); CHECK_STR(b, "   ab"); }
TC_CASE(wide_hash_S) { wchar_t b[32]; _snwprintf(b, 32, L"%#S", sab); CHECK_WSTR(b, L"ab"); }

int main(void) { tc_begin(); TC_RUN(hash_c); TC_RUN(hash_s); TC_RUN(hash_s_width); TC_RUN(wide_hash_S); return tc_end(); }
