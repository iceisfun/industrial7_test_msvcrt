/* TC
 * id:    sec_neg_splitpath_s_cleanup_probe
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _splitpath_s
 * note:  PROBE: state of component buffers on error. Believed: after ERANGE all supplied component buffers are set to
 *        empty strings; after a NULL-path EINVAL the buffers are left untouched (the CRT cannot know they are valid).
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
TC_CASE(untouched_buffers_after_null_path)
{
    char dr[8]; int r;
    memset(dr, 'x', sizeof dr);
    r = _splitpath_s(NULL, dr, 8, NULL, 0, NULL, 0, NULL, 0);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(dr[0], 'x');
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(all_buffers_emptied_after_range_error); TC_RUN(untouched_buffers_after_null_path);
    return tc_end();
}
