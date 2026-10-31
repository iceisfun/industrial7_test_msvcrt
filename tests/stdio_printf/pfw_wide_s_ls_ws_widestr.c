/* TC
 * id:    pfw_wide_s_ls_ws_widestr
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters: in wide functions s = wide string, ls/ws = wide string
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const wchar_t * volatile ws = L"abc";
TC_CASE(s_wide)  { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%s]", ws); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(ls_wide) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%ls]", ws); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(ws_wide) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%ws]", ws); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(w_prefix_mix) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"%s|%ls|%ws", ws, ws, ws); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"abc|abc|abc"); CHECK_EQ_I(n, 11); }
int main(void) { tc_begin(); TC_RUN(s_wide); TC_RUN(ls_wide); TC_RUN(ws_wide); TC_RUN(w_prefix_mix); return tc_end(); }
