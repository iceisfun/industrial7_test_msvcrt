/* TC
 * id:    pfr_vscprintf_basic
 * area:  stdio_printf
 * funcs: _vscprintf _vsnprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vscprintf, _vsnprintf
 * note:  _vscprintf is documented from VS2005 (msvcr80+); msvcrt.dll export is probed separately (pfr_scprintf_msvcrt_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int sc(const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int sp(char *b, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsprintf(b, f, ap); va_end(ap); return r; }
static int vn(char *b, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, c, f, ap); va_end(ap); return r; }

TC_CASE(matches) { char b[64]; volatile int v = 77; int a = sc("%d:%s:%x", v, "abc", 255); int c = sp(b, "%d:%s:%x", v, "abc", 255); CHECK_EQ_I(a, c); CHECK_EQ_I(a, 9); }
TC_CASE(two_pass) { char b[64]; volatile int v = 2024; int n = sc("year=%d", v); int r; CHECK_EQ_I(n, 9); b[n + 1] = 'Q'; r = vn(b, (size_t)n + 1, "year=%d", v); CHECK_EQ_I(r, 9); CHECK_STR(b, "year=2024"); }
TC_CASE(empty) { volatile const char *f = ""; CHECK_EQ_I(sc((const char *)f), 0); }
TC_CASE(long_out) { volatile int v = 1; CHECK_EQ_I(sc("%5000d", v), 5000); }

int main(void) { tc_begin(); TC_RUN(matches); TC_RUN(two_pass); TC_RUN(empty); TC_RUN(long_out); return tc_end(); }
