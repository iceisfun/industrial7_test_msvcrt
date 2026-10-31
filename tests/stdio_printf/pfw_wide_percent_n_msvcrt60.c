/* TC
 * id:    pfw_wide_percent_n_msvcrt60
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  observed
 * ref:   msvcrt.dll predates the %n security change (VS2005); %n is honoured without any opt-in
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(percent_n_stores_count_no_handler) { wchar_t b[32]; int n = -1, r = _snwprintf(b, 32, L"ab%ncd", &n); CHECK_EQ_I(n, 2); CHECK_EQ_I(r, 4); CHECK_WSTR(b, L"abcd"); }

int main(void) { tc_begin(); TC_RUN(percent_n_stores_count_no_handler); return tc_end(); }
