/* TC
 * id:    pfs_neg_swprintf_count_truncate
 * area:  stdio_printf
 * funcs: swprintf _snwprintf
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf: returns negative on overflow of count; _snwprintf: -1 when output exceeds count
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *vw = L"abcdef";

TC_CASE(swprintf_too_small) {
  wchar_t b[8]; int n; int i; for (i = 0; i < 8; i++) b[i] = L'Z';
  n = swprintf(b, 3, L"%ls", (const wchar_t *)vw);
  CHECK(n < 0);
  CHECK_EQ_I(b[2], 0);  /* swprintf always NUL-terminates within count */
  CHECK_EQ_I(b[3], L'Z');
}
TC_CASE(snwprintf_too_small) {
  wchar_t b[8]; int n; int i; for (i = 0; i < 8; i++) b[i] = L'Z';
  n = _snwprintf(b, 3, L"%ls", (const wchar_t *)vw);
  CHECK_EQ_I(n, -1);
  CHECK(b[0]==L'a' && b[1]==L'b' && b[2]==L'c'); CHECK_EQ_I(b[3], L'Z');
}
int main(void) { tc_begin(); TC_RUN(swprintf_too_small); TC_RUN(snwprintf_too_small); return tc_end(); }
