/* TC
 * id:    fio_neg_probe_wremove_null
 * area:  stdio_file
 * funcs: _wremove
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs: parameter validation
 * note:  PROBE: _wremove(NULL) invokes the invalid-parameter handler once, returns -1, errno EINVAL (msvcr80+).
 *        TRIAGE win10, class A): ucrtbase 10.0.19045 _wremove(NULL) returns -1 with errno ENOENT (2) and no handler call (observed got=2, iph 0).
 *        Old EINVAL+handler expectation kept for non-UCRT targets (unverified).
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
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, ENOENT);             /* observed ucrtbase 10.0.19045: no validation, errno from the failed delete */
    CHECK_EQ_I(tc_iph_count, 0);
#else
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
#endif
}
int main(void) { tc_begin(); TC_RUN(null_name); return tc_end(); }
