/* TC
 * id:    pfs_vsnprintf_str_exact_fit
 * area:  stdio_printf
 * funcs: _vsnprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnprintf: len == count returns len without NUL terminator
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>

static int vs_(char *b, size_t n, const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = _vsnprintf(b, n, fmt, ap); va_end(ap); return r; }
static volatile const char *vs = "abc";
static volatile const wchar_t *vw = L"abc";

TC_CASE(room) { char b[16]; int n; memset(b, 'Z', sizeof b); n = vs_(b, 8, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(exact) { char b[16]; int n; memset(b, 'Z', sizeof b); n = vs_(b, 3, "%s", (const char *)vs); CHECK_EQ_I(n, 3); CHECK(b[0]=='a' && b[1]=='b' && b[2]=='c'); CHECK_EQ_I(b[3], 'Z'); }
TC_CASE(ls) { char b[16]; int n; memset(b, 'Z', sizeof b); n = vs_(b, 3, "%ls", (const wchar_t *)vw); CHECK_EQ_I(n, 3); CHECK(b[0]=='a' && b[2]=='c'); CHECK_EQ_I(b[3], 'Z'); }
TC_CASE(lc) { char b[16]; int n; memset(b, 'Z', sizeof b); n = vs_(b, 8, "%lc", (wint_t)L'q'); CHECK_EQ_I(n, 1); CHECK_STR(b, "q"); }
int main(void) { tc_begin(); TC_RUN(room); TC_RUN(exact); TC_RUN(ls); TC_RUN(lc); return tc_end(); }
