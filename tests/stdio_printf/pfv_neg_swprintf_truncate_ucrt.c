/* TC
 * id:    pfv_neg_swprintf_truncate_ucrt
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf; MS docs swprintf (UCRT conforming form)
 * note:  UCRT swprintf(buf,n,fmt) is the ISO form.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

TC_CASE(ret_negative) { wchar_t b[16]; volatile int n = swprintf(b, 4, L"%ls", L"abcdef"); CHECK(n < 0); }
TC_CASE(nul_terminated) { wchar_t b[16]; volatile int n; int i; for (i = 0; i < 16; i++) b[i] = L'x'; n = swprintf(b, 4, L"%ls", L"abcdef"); CHECK(n < 0);
    CHECK_WSTR(b, L"abc"); }
TC_CASE(fits) { wchar_t b[16]; volatile int n = swprintf(b, 8, L"%ls", L"abc"); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); }

int main(void) { tc_begin(); TC_RUN(ret_negative); TC_RUN(nul_terminated); TC_RUN(fits); return tc_end(); }
