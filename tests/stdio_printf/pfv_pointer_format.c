/* TC
 * id:    pfv_pointer_format
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf %p (hex digits, uppercase)
 * note:  Pointer-width zero-padded uppercase hex, no 0x prefix; NULL prints zeros.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#if TC_X64
#define Z8 L"00000000"
#define S8 "00000000"
#else
#define Z8 L""
#define S8 ""
#endif
static char b[64]; static wchar_t w[64];
TC_CASE(small_value) { void *volatile p = (void *)(size_t)0x1234; sprintf(b, "%p", p); CHECK_STR(b, S8 "00001234"); }
TC_CASE(null_ptr) { void *volatile p = NULL; sprintf(b, "%p", p); CHECK_STR(b, S8 "00000000"); }
TC_CASE(mixed_hex_case) { void *volatile p = (void *)(size_t)0xABCDEF; sprintf(b, "%p", p); CHECK_STR(b, S8 "00ABCDEF"); }
TC_CASE(wide_variant) { void *volatile p = (void *)(size_t)0xABCDEF; _snwprintf(w, 63, L"%p", p); w[63] = 0; CHECK_WSTR(w, Z8 L"00ABCDEF"); }

int main(void) { tc_begin(); TC_RUN(small_value); TC_RUN(null_ptr); TC_RUN(mixed_hex_case); TC_RUN(wide_variant); return tc_end(); }
