/* TC
 * id:    pfm_1_bare_dot_c_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf precision specification (precision has no effect for c, C)
 * note:  PROBE: a bare '.' (precision 0) with %c is ignored, so "%.c" of 'x' prints "x" (C leaves this undefined).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow) { char b[16]; volatile int c = 'x'; sprintf(b, "[%.c]", c); CHECK_STR(b, "[x]"); sprintf(b, "[%.0c]", c); CHECK_STR(b, "[x]"); }
TC_CASE(wide) { wchar_t w[16]; volatile int c = 'x'; _snwprintf(w, 16, L"[%.c]", c); CHECK_WSTR(w, L"[x]"); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
