/* TC
 * id:    pfv_narrow_sprintf_wide_specs
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf type field and size prefixes
 * note:  Wide args converted to multibyte; ASCII in C locale.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64];
static const wchar_t *ws = L"abcd";
TC_CASE(ls_wide_to_narrow) { sprintf(b, "[%ls]", ws); CHECK_STR(b, "[abcd]"); }
TC_CASE(S_wide_to_narrow) { sprintf(b, "[%S]", ws); CHECK_STR(b, "[abcd]"); }
TC_CASE(lc_char) { volatile wchar_t w = L'Q'; sprintf(b, "[%lc]", (int)w); CHECK_STR(b, "[Q]"); }
TC_CASE(C_wide_char) { volatile wchar_t w = L'R'; sprintf(b, "[%C]", (int)w); CHECK_STR(b, "[R]"); }
TC_CASE(precision_ls) { sprintf(b, "[%.2ls]", ws); CHECK_STR(b, "[ab]"); }

int main(void) { tc_begin(); TC_RUN(ls_wide_to_narrow); TC_RUN(S_wide_to_narrow); TC_RUN(lc_char); TC_RUN(C_wide_char); TC_RUN(precision_ls); return tc_end(); }
