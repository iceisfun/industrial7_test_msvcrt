/* TC
 * id:    pfr_neg_snprintf_legacy_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf: if len > count, count chars stored (no NUL) and a negative value is returned
 * note:  Return is asserted only as negative (-1 in practice); nothing is written beyond count.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow_neg) { char b[16]; volatile size_t c = 4; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z'; n = _snprintf(b, c, "%s", "abcdefgh"); CHECK(n < 0); CHECK_MEM(b, "abcd", 4); for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z'); }
TC_CASE(wide_neg) { wchar_t b[16]; volatile size_t c = 3; int n, i; for (i = 0; i < 16; i++) b[i] = L'Z'; n = _snwprintf(b, c, L"%s", L"abcdefgh"); CHECK(n < 0); CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[2], L'c'); for (i = 3; i < 16; i++) CHECK_EQ_I(b[i], L'Z'); }
TC_CASE(by_one) { char b[16]; volatile size_t c = 5; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z'; n = _snprintf(b, c, "%s", "abcdef"); CHECK(n < 0); CHECK_EQ_I(b[5], 'Z'); }

int main(void) { tc_begin(); TC_RUN(narrow_neg); TC_RUN(wide_neg); TC_RUN(by_one); return tc_end(); }
