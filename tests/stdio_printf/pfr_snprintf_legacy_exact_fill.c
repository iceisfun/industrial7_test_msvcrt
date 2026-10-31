/* TC
 * id:    pfr_snprintf_legacy_exact_fill
 * area:  stdio_printf
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf: if len == count, len chars stored, no NUL appended, returns len
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow) { char b[16]; volatile size_t c = 5; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z'; n = _snprintf(b, c, "%s", "abcde"); CHECK_EQ_I(n, 5); CHECK_MEM(b, "abcde", 5); CHECK_EQ_I(b[5], 'Z'); }
TC_CASE(narrow_fmt) { char b[16]; volatile size_t c = 4; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z'; n = _snprintf(b, c, "%d", 1234); CHECK_EQ_I(n, 4); CHECK_MEM(b, "1234", 4); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(wide) { wchar_t b[16]; volatile size_t c = 5; int n, i; for (i = 0; i < 16; i++) b[i] = L'Z'; n = _snwprintf(b, c, L"%s", L"abcde"); CHECK_EQ_I(n, 5); CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[4], L'e'); CHECK_EQ_I(b[5], L'Z'); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(narrow_fmt); TC_RUN(wide); return tc_end(); }
