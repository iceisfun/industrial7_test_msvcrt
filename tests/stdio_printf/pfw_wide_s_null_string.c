/* TC
 * id:    pfw_wide_s_null_string
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   Windows CRT prints (null) for a NULL string argument (not specified by ISO C; undefined there)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile wn = NULL;
static const char * volatile nn = NULL;
TC_CASE(ls_null) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%ls]", wn); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[(null)]"); CHECK_EQ_I(n, 8); }
TC_CASE(S_null)  { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%S]", nn); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[(null)]"); CHECK_EQ_I(n, 8); }
TC_CASE(hs_null) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%hs]", nn); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[(null)]"); CHECK_EQ_I(n, 8); }
int main(void) { tc_begin(); TC_RUN(ls_null); TC_RUN(S_null); TC_RUN(hs_null); return tc_end(); }
