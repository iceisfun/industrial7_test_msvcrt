/* TC
 * id:    pfw_probe_wide_int_size_i32_i
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes: I32, I (ptrdiff_t/size_t by platform)
 * note:  Probes whether %I32d and bare %Id (pointer-sized) are accepted by every target, including msvcrt.dll and
 *        msvcr80, where support predates/postdates documentation is unknown. x64 build: %Id is 64-bit.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static volatile int i32 = -123456;
static volatile INT_PTR ip = -77;
TC_CASE(I32d) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%I32d", i32); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"-123456"); CHECK_EQ_I(n, 7); }
TC_CASE(Id_pointer_sized) { wchar_t b[96]; int n; fill(b); n = _snwprintf(b, 95, L"%Id", ip); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"-77"); CHECK_EQ_I(n, 3); }
int main(void) { tc_begin(); TC_RUN(I32d); TC_RUN(Id_pointer_sized); return tc_end(); }
