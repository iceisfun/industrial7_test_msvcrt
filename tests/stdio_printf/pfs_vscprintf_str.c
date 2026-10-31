/* TC
 * id:    pfs_vscprintf_str
 * area:  stdio_printf
 * funcs: _vscprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vscprintf, _vscprintf_l, _vscwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int count(const char *fmt, ...) { va_list ap; int n; va_start(ap, fmt); n = _vscprintf(fmt, ap); va_end(ap); return n; }
static volatile const char *vs = "abc";
static volatile const wchar_t *vw = L"wxyz";

TC_CASE(two_s) { CHECK_EQ_I(count("%s%s", (const char *)vs, (const char *)vs), 6); }
TC_CASE(ls) { CHECK_EQ_I(count("%ls", (const wchar_t *)vw), 4); }
TC_CASE(star_prec) { CHECK_EQ_I(count("%.*s", 2, (const char *)vs), 2); }
TC_CASE(c) { CHECK_EQ_I(count("%c%c", 'a', 0), 2); }
int main(void) { tc_begin(); TC_RUN(two_s); TC_RUN(ls); TC_RUN(star_prec); TC_RUN(c); return tc_end(); }
