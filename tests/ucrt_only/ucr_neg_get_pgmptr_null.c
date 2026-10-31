/* TC
 * id:    ucr_neg_get_pgmptr_null
 * area:  ucrt_only
 * funcs: _get_pgmptr _get_wpgmptr
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_pgmptr: NULL pointer -> invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char ** volatile vn; static wchar_t ** volatile vw;
TC_CASE(narrow) { int r; errno = 0; tc_iph_count = 0; r = _get_pgmptr(vn); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wide)   { int r; errno = 0; tc_iph_count = 0; r = _get_wpgmptr(vw); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(narrow); TC_RUN(wide);
    return tc_end();
}
