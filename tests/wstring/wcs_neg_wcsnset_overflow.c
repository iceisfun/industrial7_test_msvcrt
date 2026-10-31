/* TC
 * id:    wcs_neg_wcsnset_overflow
 * area:  wstring
 * funcs: _wcsnset
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strnset, _wcsnset
 * note:  does not write past terminator
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static void init(wchar_t *d, int n, const wchar_t *s) { int i = 0; for (; i < n; i++) d[i] = 0x5555; for (i = 0; s[i]; i++) d[i] = s[i]; d[i] = 0; }
TC_CASE(big_n) { wchar_t d[8]; init(d, 8, L"abc"); _wcsnset(d, L'z', 6); CHECK_WSTR(d, L"zzz"); CHECK_EQ_I(d[3], 0); CHECK_EQ_I(d[4], 0x5555); CHECK_EQ_I(d[5], 0x5555); }
TC_CASE(zero) { wchar_t d[8]; init(d, 8, L"abc"); _wcsnset(d, L'z', 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(partial) { wchar_t d[8]; init(d, 8, L"abc"); _wcsnset(d, L'z', 2); CHECK_WSTR(d, L"zzc"); }
int main(void) { tc_begin(); TC_RUN(big_n); TC_RUN(zero); TC_RUN(partial); return tc_end(); }
