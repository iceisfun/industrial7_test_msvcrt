/* TC
 * id:    pfm_1_len_h_c_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Type Field Characters: %c/%C/%s/%S with h / l prefixes
 * note:  PROBES the h prefix on character conversions: %hc and %hC denote a narrow char in both sprintf and wide
 *        formatters; %hs a narrow string. Expected "a" / L"a" / "ab". (_snwprintf stands in for swprintf to avoid the
 *        signature split of swprintf across CRT generations.)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow_hC)  { char b[16]; volatile int c = 'a'; sprintf(b, "%hC", c); CHECK_STR(b, "a"); }
TC_CASE(narrow_hc)  { char b[16]; volatile int c = 'a'; sprintf(b, "%hc", c); CHECK_STR(b, "a"); }
TC_CASE(narrow_hs)  { char b[16]; const char *volatile s = "ab"; sprintf(b, "%hs", s); CHECK_STR(b, "ab"); }
TC_CASE(wide_hc)    { wchar_t b[16]; volatile int c = 'a'; _snwprintf(b, 16, L"%hc", c); CHECK_WSTR(b, L"a"); }
TC_CASE(wide_hC)    { wchar_t b[16]; volatile int c = 'a'; _snwprintf(b, 16, L"%hC", c); CHECK_WSTR(b, L"a"); }
TC_CASE(wide_hs)    { wchar_t b[16]; const char *volatile s = "ab"; _snwprintf(b, 16, L"%hs", s); CHECK_WSTR(b, L"ab"); }

int main(void)
{
    tc_begin();
    TC_RUN(narrow_hC); TC_RUN(narrow_hc); TC_RUN(narrow_hs); TC_RUN(wide_hc); TC_RUN(wide_hC); TC_RUN(wide_hs);
    return tc_end();
}
