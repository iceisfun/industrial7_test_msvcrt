/* TC
 * id:    pfr_scprintf_basic
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scprintf_l
 * note:  _scprintf/_vscprintf are documented from VS2005 (msvcr80+); msvcrt.dll export is probed separately (pfr_scprintf_msvcrt_probe).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(literal) { volatile const char *f = "hello"; CHECK_EQ_I(_scprintf((const char *)f), 5); }
TC_CASE(args) { volatile int v = 12; CHECK_EQ_I(_scprintf("%d %s", v, "ab"), 5); }
TC_CASE(empty) { volatile const char *f = ""; CHECK_EQ_I(_scprintf((const char *)f), 0); }
TC_CASE(width_prec) { volatile double d = 3.14159; CHECK_EQ_I(_scprintf("%10.3f", d), 10); }
TC_CASE(matches_sprintf) { char b[64]; volatile int v = -987; int a = _scprintf("[%6d|%-4s]", v, "x"); int c = sprintf(b, "[%6d|%-4s]", v, "x"); CHECK_EQ_I(a, c); CHECK_EQ_I(a, 13); }

int main(void) { tc_begin(); TC_RUN(literal); TC_RUN(args); TC_RUN(empty); TC_RUN(width_prec); TC_RUN(matches_sprintf); return tc_end(); }
