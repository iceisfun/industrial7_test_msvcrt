/* TC
 * id:    sec_neg_splitpath_s_args
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _splitpath_s, _wsplitpath_s (Error conditions table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static const char *volatile v_path = "C:\\dir\\name.ext";
static char *volatile v_null_buf = NULL;
static volatile size_t v_zero = 0, v_ten = 10;

TC_CASE(null_path_EINVAL) { char d[8]; int r; errno = 0; r = _splitpath_s(NULL, d, 8, NULL, 0, NULL, 0, NULL, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(buffer_nonnull_size_zero_EINVAL) { char d[8]; int r; errno = 0; r = _splitpath_s(v_path, d, v_zero, NULL, 0, NULL, 0, NULL, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(buffer_null_size_nonzero_EINVAL) { int r; errno = 0; r = _splitpath_s(v_path, v_null_buf, v_ten, NULL, 0, NULL, 0, NULL, 0); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts)
{
    char d[8]; tc_iph_count = 0;
    _splitpath_s(NULL, d, 8, NULL, 0, NULL, 0, NULL, 0);
    _splitpath_s(v_path, d, v_zero, NULL, 0, NULL, 0, NULL, 0);
    _splitpath_s(v_path, v_null_buf, v_ten, NULL, 0, NULL, 0, NULL, 0);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_path_EINVAL); TC_RUN(buffer_nonnull_size_zero_EINVAL); TC_RUN(buffer_null_size_nonzero_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
