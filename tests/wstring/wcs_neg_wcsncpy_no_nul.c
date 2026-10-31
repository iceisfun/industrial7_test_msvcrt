/* TC
 * id:    wcs_neg_wcsncpy_no_nul
 * area:  wstring
 * funcs: wcsncpy
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.2.2; MS docs wcsncpy
 * note:  must-not-write guard
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t *volatile ps = L"abcd";
static void fill(wchar_t *d, int n) { int i; for (i = 0; i < n; i++) d[i] = 0x5555; }
TC_CASE(trunc) { wchar_t d[8]; fill(d, 8); wcsncpy(d, ps, 3); CHECK_EQ_I(d[3], 0x5555); CHECK_EQ_I(d[4], 0x5555); }
TC_CASE(last_char) { wchar_t d[8]; fill(d, 8); wcsncpy(d, ps, 3); CHECK_EQ_I(d[2], 'c'); CHECK(d[2] != 0); }
TC_CASE(n_eq_len) { wchar_t d[8]; fill(d, 8); wcsncpy(d, ps, 4); CHECK_EQ_I(d[3], 'd'); CHECK_EQ_I(d[4], 0x5555); }
int main(void) { tc_begin(); TC_RUN(trunc); TC_RUN(last_char); TC_RUN(n_eq_len); return tc_end(); }
