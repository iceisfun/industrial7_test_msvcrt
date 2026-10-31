/* TC
 * id:    fio_neg_probe_wremove_null
 * area:  stdio_file
 * funcs: _wremove
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: parameter validation
 * note:  PROBE: _wremove(NULL) invokes the invalid-parameter handler once, returns -1, errno EINVAL (msvcr80+).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
TC_CASE(null_name)
{
    const wchar_t * volatile p = NULL;
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _wremove(p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_name); return tc_end(); }
