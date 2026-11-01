/* TC
 * id:    sec_neg_splitpath_s_cleanup_probe
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _splitpath_s
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): after _splitpath_s(NULL,dr,8,...) returns EINVAL, dr[0] is 0 (supplied component
 *        buffers are set to empty strings, not left untouched); after an ERANGE all four buffers are emptied too (that case
 *        already matched). The earlier "untouched" belief was wrong. Stable.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static const char *volatile v_path = "C:\\dir\\name.ext";

TC_CASE(all_buffers_emptied_after_range_error)
{
    char dr[8], di[16], fn[4], ex[16]; int r;
    memset(dr, 'x', sizeof dr); memset(di, 'x', sizeof di); memset(fn, 'x', sizeof fn); memset(ex, 'x', sizeof ex);
    r = _splitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(dr[0], 0); CHECK_EQ_I(di[0], 0); CHECK_EQ_I(fn[0], 0); CHECK_EQ_I(ex[0], 0);
}
TC_CASE(emptied_buffers_after_null_path)
{
    char dr[8]; int r;
    memset(dr, 'x', sizeof dr);
    r = _splitpath_s(NULL, dr, 8, NULL, 0, NULL, 0, NULL, 0);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(dr[0], 0);   /* observed: the supplied drive buffer IS emptied even for a NULL path */
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(all_buffers_emptied_after_range_error); TC_RUN(emptied_buffers_after_null_path);
    return tc_end();
}
