/* TC
 * id:    wcs_neg_wcsncpy_s_null_src_count0
 * area:  wstring
 * funcs: 
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s error table
 * note:  PROBES: wcsncpy_s(dest,size,NULL,0) -- most likely returns 0 with dest[0]==0 and no handler call (count==0 is handled before src check). Only the count==0 NULL-src path is probed; the documented NULL-src count>0 case lives in wcs_neg_wcsncpy_s.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
TC_CASE(null_src_count0) { wchar_t d[4]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); d[0] = 'x'; tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 4, NULL, 0); CHECK_EQ_I(r, 0); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncpy_s); TC_RUN(null_src_count0); return tc_end(); }
