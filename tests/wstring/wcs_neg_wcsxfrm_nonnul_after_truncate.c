/* TC
 * id:    wcs_neg_wcsxfrm_nonnul_after_truncate
 * area:  wstring
 * funcs: wcsxfrm
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.4.4: if the returned value is >= n, contents of the array are indeterminate; at most n elements written
 * note:  Only the return value and the no-write-beyond-n guarantee are asserted, never the contents.
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(return_signals_truncation)
{
    const wchar_t * volatile s = L"abcdefgh"; wchar_t d[16] = {0}; size_t n;
    for (n = 1; n <= 8; n++) CHECK(wcsxfrm(d, s, n) >= n);   /* len 8: truncated for every n <= 8 */
    CHECK(wcsxfrm(d, s, 9) < 9);
}
TC_CASE(no_write_beyond_n)
{
    const wchar_t * volatile s = L"abcdefgh"; wchar_t d[16]; int i;
    for (i = 0; i < 16; i++) d[i] = 0x5A5A;
    wcsxfrm(d, s, 3);
    for (i = 3; i < 16; i++) CHECK_EQ_I(d[i], 0x5A5A);
}
int main(void) { tc_begin(); TC_RUN(return_signals_truncation); TC_RUN(no_write_beyond_n); return tc_end(); }
