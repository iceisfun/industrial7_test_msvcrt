/* TC
 * id:    wcs_neg_wcsncat_bounds
 * area:  wstring
 * funcs: wcsncat
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.3.2; MS docs wcsncat
 * note:  must-not-write guard
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t *volatile ps = L"cdefg";
static void init(wchar_t *d, int n, const wchar_t *s) { int i = 0; for (; i < n; i++) d[i] = 0x5555; for (i = 0; s[i]; i++) d[i] = s[i]; d[i] = 0; }
TC_CASE(n2) { wchar_t d[10]; init(d, 10, L"ab"); wcsncat(d, ps, 2); CHECK_WSTR(d, L"abcd"); CHECK_EQ_I(d[5], 0x5555); CHECK_EQ_I(d[6], 0x5555); }
TC_CASE(n0) { wchar_t d[10]; init(d, 10, L"ab"); wcsncat(d, ps, 0); CHECK_WSTR(d, L"ab"); CHECK_EQ_I(d[3], 0x5555); }
int main(void) { tc_begin(); TC_RUN(n2); TC_RUN(n0); return tc_end(); }
