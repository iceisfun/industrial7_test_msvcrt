/* TC
 * id:    pfv_wide_s_wprintf_family
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field and size prefixes
 * note:  wprintf family: %s wide, %S narrow. ASCII only, C locale.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static wchar_t b[64];
#define F1(fmt, arg) do { int i_; for (i_ = 0; i_ < 64; i_++) b[i_] = 0; _snwprintf(b, 63, fmt, arg); } while (0)
static const wchar_t *ws = L"wide"; static const char *ns = "narrow";
TC_CASE(s_is_wide) { F1(L"[%s]", ws); CHECK_WSTR(b, L"[wide]"); }
TC_CASE(S_is_narrow) { F1(L"[%S]", ns); CHECK_WSTR(b, L"[narrow]"); }
TC_CASE(hs_narrow) { F1(L"[%hs]", ns); CHECK_WSTR(b, L"[narrow]"); }
TC_CASE(ls_wide) { F1(L"[%ls]", ws); CHECK_WSTR(b, L"[wide]"); }
TC_CASE(c_is_wide) { volatile wchar_t c = L'W'; F1(L"[%c]", (int)c); CHECK_WSTR(b, L"[W]"); }
TC_CASE(C_is_narrow) { volatile char c = 'N'; F1(L"[%C]", (int)c); CHECK_WSTR(b, L"[N]"); }
TC_CASE(hc_lc) { volatile char c = 'h'; volatile wchar_t w = L'l'; _snwprintf(b, 63, L"[%hc][%lc]", (int)c, (int)w); CHECK_WSTR(b, L"[h][l]"); }

int main(void) { tc_begin(); TC_RUN(s_is_wide); TC_RUN(S_is_narrow); TC_RUN(hs_narrow); TC_RUN(ls_wide); TC_RUN(c_is_wide); TC_RUN(C_is_narrow); TC_RUN(hc_lc); return tc_end(); }
