/* TC
 * id:    sec_neg_wsplitpath_s_args
 * area:  secure_crt
 * funcs: _wsplitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _splitpath_s, _wsplitpath_s
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static const wchar_t *volatile v_path = L"C:\\dir\\name.ext";
static volatile size_t v_zero = 0;

TC_CASE(null_path_EINVAL) { wchar_t d[8]; int r; errno = 0; r = _wsplitpath_s(NULL, d, 8, NULL, 0, NULL, 0, NULL, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(buffer_nonnull_size_zero_EINVAL) { wchar_t d[8]; int r; errno = 0; r = _wsplitpath_s(v_path, d, v_zero, NULL, 0, NULL, 0, NULL, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fname_too_small_ERANGE) { wchar_t dr[8], di[16], fn[4], ex[16]; int r; errno = 0; r = _wsplitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_counts)
{
    wchar_t d[8], dr[8], di[16], fn[4], ex[16]; tc_iph_count = 0;
    _wsplitpath_s(NULL, d, 8, NULL, 0, NULL, 0, NULL, 0);
    _wsplitpath_s(v_path, d, v_zero, NULL, 0, NULL, 0, NULL, 0);
    _wsplitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_path_EINVAL); TC_RUN(buffer_nonnull_size_zero_EINVAL); TC_RUN(fname_too_small_ERANGE); TC_RUN(handler_counts);
    return tc_end();
}
