/* TC
 * id:    pfw_vswprintf_legacy_basic
 * area:  stdio_printf
 * funcs: _vswprintf
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs vswprintf, _vswprintf (non-count form)
 * note:  _vswprintf(buf, fmt, va_list)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int vs(wchar_t *b, const wchar_t *f, ...)
{ va_list ap; int n; va_start(ap, f); n = _vswprintf(b, f, ap); va_end(ap); return n; }

TC_CASE(basic)      { wchar_t b[32]; fill(b, 32); vs(b, L"%d+%ls", 3, L"x"); CHECK_WSTR(b, L"3+x"); }
TC_CASE(return_len) { wchar_t b[32]; fill(b, 32); CHECK_EQ_I(vs(b, L"%ls%d", L"abc", 1234), 7); }
TC_CASE(NUL_written){ wchar_t b[8]; fill(b, 8); vs(b, L"hi"); CHECK(b[2] == 0); CHECK(b[3] == L'Z'); }

int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(return_len); TC_RUN(NUL_written); return tc_end(); }
