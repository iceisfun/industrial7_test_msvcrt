/* TC
 * id:    pfr_snprintf_legacy_count_zero
 * area:  stdio_printf
 * funcs: _snprintf _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf (empty output, count 0: nothing to store)
 * note:  Only the empty-output case is exercised; non-empty with count 0 differs between CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(narrow) { char b[4]; volatile size_t c = 0; volatile const char *f = ""; int n; b[0] = 'Z'; b[1] = 'Z'; n = _snprintf(b, c, (const char *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[1], 'Z'); }
TC_CASE(wide) { wchar_t b[4]; volatile size_t c = 0; volatile const wchar_t *f = L""; int n; b[0] = L'Z'; n = _snwprintf(b, c, (const wchar_t *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], L'Z'); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
