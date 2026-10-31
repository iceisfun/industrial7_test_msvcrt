/* TC
 * id:    sec_neg_getenv_s_args
 * area:  secure_crt
 * funcs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s, _wgetenv_s (Error conditions table)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static const char *volatile v_name = "TC_SURELY_UNSET_VARIABLE_XYZ";
static size_t g_ret;

TC_CASE(null_retsize_ptr_EINVAL) { char b[8]; int r; errno = 0; r = getenv_s(NULL, b, 8, v_name); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_buffer_nonzero_size_EINVAL) { int r; errno = 0; r = getenv_s(&g_ret, NULL, 8, v_name); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_counts)
{
    char b[8]; tc_iph_count = 0;
    getenv_s(NULL, b, 8, v_name);
    getenv_s(&g_ret, NULL, 8, v_name);
    CHECK_EQ_I(tc_iph_count, 2);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_retsize_ptr_EINVAL); TC_RUN(null_buffer_nonzero_size_EINVAL); TC_RUN(handler_counts);
    return tc_end();
}
