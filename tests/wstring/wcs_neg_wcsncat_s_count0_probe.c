/* TC
 * id:    wcs_neg_wcsncat_s_count0_probe
 * area:  wstring
 * funcs: 
 * procs: wcsncat_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncat_s (count==0 behaviour not stated)
 * note:  PROBES: wcsncat_s(valid dest "ab", size 6, src, count 0) returns 0, leaves dest "ab", no handler call. Believed true but undocumented.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcsncat_s, (wchar_t *, size_t, const wchar_t *, size_t));
static const wchar_t *volatile pl = L"cdefg";
TC_CASE(count0) { wchar_t d[6] = {'a', 'b', 0, 0x5555, 0x5555, 0x5555}; int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcsncat_s(d, 6, pl, 0); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"ab"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncat_s); TC_RUN(count0); return tc_end(); }
