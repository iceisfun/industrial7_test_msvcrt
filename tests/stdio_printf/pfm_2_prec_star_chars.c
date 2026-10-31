/* TC
 * id:    pfm_2_prec_star_chars
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf precision specification: precision has no effect on c and C
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[32]; static wchar_t w[32];
TC_CASE(star_c)    { volatile int c = 'x'; sprintf(b, "%.*c", 3, c); CHECK_STR(b, "x"); }
TC_CASE(zero_c)    { volatile int c = 'x'; sprintf(b, "%.0c", c); CHECK_STR(b, "x"); }
TC_CASE(two_C)     { volatile wchar_t c = L'x'; sprintf(b, "%.2C", (int)c); CHECK_STR(b, "x"); }
TC_CASE(star0_C)   { volatile wchar_t c = L'y'; sprintf(b, "%.*C", 0, (int)c); CHECK_STR(b, "y"); }
TC_CASE(wide_c)    { volatile wchar_t c = L'x'; _snwprintf(w, 31, L"%.*c", 3, (int)c); CHECK_WSTR(w, L"x"); }
TC_CASE(wide_zero_c) { volatile wchar_t c = L'x'; _snwprintf(w, 31, L"%.0c", (int)c); CHECK_WSTR(w, L"x"); }
TC_CASE(wide_C)    { volatile int c = 'y'; _snwprintf(w, 31, L"%.2C", c); CHECK_WSTR(w, L"y"); }
TC_CASE(width_with_prec) { volatile int c = 'x'; sprintf(b, "[%3.0c]", c); CHECK_STR(b, "[  x]"); }
int main(void)
{
    tc_begin();
    TC_RUN(star_c); TC_RUN(zero_c); TC_RUN(two_C); TC_RUN(star0_C);
    TC_RUN(wide_c); TC_RUN(wide_zero_c); TC_RUN(wide_C); TC_RUN(width_with_prec);
    return tc_end();
}
