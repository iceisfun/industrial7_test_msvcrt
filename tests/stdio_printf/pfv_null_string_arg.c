/* TC
 * id:    pfv_null_string_arg
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS CRT prints (null) for NULL string args (undefined in ISO C)
 * note:  Observed behaviour of the Microsoft CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64]; static wchar_t w[64];
static const char *volatile np; static const wchar_t *volatile wp;
TC_CASE(s_null) { sprintf(b, "[%s]", np); CHECK_STR(b, "[(null)]"); }
TC_CASE(ls_null_narrow) { sprintf(b, "[%ls]", wp); CHECK_STR(b, "[(null)]"); }
TC_CASE(wide_s_null) { _snwprintf(w, 63, L"[%s]", wp); w[63] = 0; CHECK_WSTR(w, L"[(null)]"); }
TC_CASE(wide_hs_null) { _snwprintf(w, 63, L"[%hs]", np); w[63] = 0; CHECK_WSTR(w, L"[(null)]"); }

int main(void) { tc_begin(); TC_RUN(s_null); TC_RUN(ls_null_narrow); TC_RUN(wide_s_null); TC_RUN(wide_hs_null); return tc_end(); }
