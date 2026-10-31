/* TC
 * id:    pfv_probe_snprintf_exact_fit_no_nul
 * area:  stdio_printf
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _snprintf: if len == count, len bytes stored with no terminating null
 * note:  PROBE: when output exactly fills count, no NUL is written (byte after stays untouched). UCRT's legacy
 *        null-termination flag may change this; real Windows decides.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

TC_CASE(narrow) { char b[16]; volatile int n; memset(b, 'x', sizeof b); n = _snprintf(b, 4, "%s", "abcd"); CHECK_EQ_I(n, 4); CHECK_EQ_I(b[4], 'x'); }
TC_CASE(wide) { wchar_t b[16]; volatile int n; int i; for (i = 0; i < 16; i++) b[i] = L'x'; n = _snwprintf(b, 4, L"%ls", L"abcd"); CHECK_EQ_I(n, 4); CHECK(b[4] == L'x'); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
