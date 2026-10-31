/* TC
 * id:    pfv_percent_w_modifier
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf size prefixes (w = wide for c, C, s, S)
 * note:  msvcrt.dll excluded (not documented there).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64]; static wchar_t w[64];
static const wchar_t *ws = L"abc";
TC_CASE(ws_narrow_out) { sprintf(b, "[%ws]", ws); CHECK_STR(b, "[abc]"); }
TC_CASE(wc_narrow_out) { volatile wchar_t c = L'Z'; sprintf(b, "[%wc]", (int)c); CHECK_STR(b, "[Z]"); }
TC_CASE(ws_wide_out) { _snwprintf(w, 63, L"[%ws]", ws); w[63] = 0; CHECK_WSTR(w, L"[abc]"); }
TC_CASE(wc_wide_out) { volatile wchar_t c = L'Z'; _snwprintf(w, 63, L"[%wc]", (int)c); w[63] = 0; CHECK_WSTR(w, L"[Z]"); }

int main(void) { tc_begin(); TC_RUN(ws_narrow_out); TC_RUN(wc_narrow_out); TC_RUN(ws_wide_out); TC_RUN(wc_wide_out); return tc_end(); }
