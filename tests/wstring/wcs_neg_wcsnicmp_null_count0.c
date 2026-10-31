/* TC
 * id:    wcs_neg_wcsnicmp_null_count0
 * area:  wstring
 * funcs: _wcsnicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcsnicmp
 * note:  PROBE: _wcsnicmp(NULL,NULL,0). The CRT likely returns 0 early when count==0 before parameter validation; only the return value 0 is asserted. The handler count is not asserted.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vn = NULL;
TC_CASE(null_null_0) { int r; tc_iph_install(); tc_iph_count = 0; r = _wcsnicmp(vn, vn, 0); CHECK_EQ_I(r, 0); }
int main(void) { tc_begin(); TC_RUN(null_null_0); return tc_end(); }
