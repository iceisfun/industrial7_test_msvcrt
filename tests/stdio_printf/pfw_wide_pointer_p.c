/* TC
 * id:    pfw_wide_pointer_p
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field p: "Displays the argument as an address in hexadecimal digits" (uppercase, zero padded to pointer width on MS CRTs)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#if TC_X64
#define P(s) L"00000000" s
#else
#define P(s) s
#endif

TC_CASE(small_value) { wchar_t b[64]; volatile uintptr_t u = 0x1234; _snwprintf(b, 64, L"%p", (void *)u); CHECK_WSTR(b, P(L"00001234")); }
TC_CASE(null_all_zero) { wchar_t b[64]; volatile uintptr_t u = 0; _snwprintf(b, 64, L"%p", (void *)u); CHECK_WSTR(b, P(L"00000000")); }
TC_CASE(uppercase) { wchar_t b[64]; volatile uintptr_t u = 0xABCDEF; _snwprintf(b, 64, L"%p", (void *)u); CHECK_WSTR(b, P(L"00ABCDEF")); }

int main(void) { tc_begin(); TC_RUN(small_value); TC_RUN(null_all_zero); TC_RUN(uppercase); return tc_end(); }
