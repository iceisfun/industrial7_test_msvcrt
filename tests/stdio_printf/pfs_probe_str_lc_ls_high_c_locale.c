/* TC
 * id:    pfs_probe_str_lc_ls_high_c_locale
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field characters
 * note:  PROBE: C locale wide chars 0x80-0xFF in narrow printf map 1:1 to bytes (UCRT); older CRTs may give '?' or fail.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile wchar_t we9 = 0xe9;
static const wchar_t *volatile wstr = L"\xe9\xff";

TC_CASE(lc) { char b[16]; int n = sprintf(b, "%lc", we9); CHECK_EQ_I(n, 1); CHECK_EQ_U((unsigned char)b[0], 0xe9); }
TC_CASE(ls) { char b[16]; int n = sprintf(b, "%ls", wstr); CHECK_EQ_I(n, 2); CHECK_EQ_U((unsigned char)b[0], 0xe9); CHECK_EQ_U((unsigned char)b[1], 0xff); }

int main(void) { tc_begin(); TC_RUN(lc); TC_RUN(ls); return tc_end(); }
