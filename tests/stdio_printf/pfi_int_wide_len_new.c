/* TC
 * id:    pfi_int_wide_len_new
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1 hh j z t; MS docs printf Size Prefixes (UCRT); I32 documented
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdint.h>

TC_CASE(hh) { wchar_t b[64]; _snwprintf(b, 64, L"%hhd|%hhu|%hhd", 300, -1, 127); CHECK_WSTR(b, L"44|255|127"); }
TC_CASE(j) { wchar_t b[64]; volatile intmax_t mn = INTMAX_MIN; volatile uintmax_t mx = UINTMAX_MAX; _snwprintf(b, 64, L"%jd|%ju", mn, mx); CHECK_WSTR(b, L"-9223372036854775808|18446744073709551615"); }
TC_CASE(z) { wchar_t b[64]; volatile size_t v = (size_t)-1; _snwprintf(b, 64, L"%zu|%zx", v, v); 
#ifdef _WIN64
 CHECK_WSTR(b, L"18446744073709551615|ffffffffffffffff");
#else
 CHECK_WSTR(b, L"4294967295|ffffffff");
#endif
}
TC_CASE(t) { wchar_t b[64]; volatile ptrdiff_t v = -17; _snwprintf(b, 64, L"%td|%tx", v, (ptrdiff_t)255); CHECK_WSTR(b, L"-17|ff"); }
TC_CASE(I32) { wchar_t b[64]; _snwprintf(b, 64, L"%I32d|%I32u|%I32x", -5, 4000000000u, 0xabcu); CHECK_WSTR(b, L"-5|4000000000|abc"); }
int main(void) { tc_begin(); TC_RUN(hh); TC_RUN(j); TC_RUN(z); TC_RUN(t); TC_RUN(I32); return tc_end(); }
