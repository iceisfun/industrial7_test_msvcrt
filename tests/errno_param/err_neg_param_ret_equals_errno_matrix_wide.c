/* TC
 * id:    err_neg_param_ret_equals_errno_matrix_wide
 * area:  errno_param
 * kind:  negative
 * funcs: wcscat_s wmemcpy_s wmemmove_s wcsncpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcat_s/strncat_s/wcscat_s/memmove_s/wmemcpy_s/wmemmove_s/strncpy_s/wcsncpy_s/_strlwr_s/_strupr_s (error condition tables: EINVAL / ERANGE)
 * note:  Each failing call must return the same value it stores in errno and invoke the handler once. errno is preset to a sentinel (4321).
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>

static char * volatile v_null = NULL;
static wchar_t * volatile v_wnull = NULL;
static volatile size_t v_two = 2;

#define PRE() do { errno = 4321; tc_iph_count = 0; } while (0)
#define POST(r, want) do { CHECK_EQ_I(r, want); CHECK_EQ_I(errno, want); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(wcscat_s_)  { wchar_t d[4] = { 0 }; int r; PRE(); r = wcscat_s(v_wnull, 10, L"a"); POST(r, EINVAL);
                      PRE(); r = wcscat_s(d, v_two, L"abc"); POST(r, ERANGE); }
TC_CASE(wmemcpy_s_) { wchar_t d[4] = { 0 }; int r; PRE(); r = wmemcpy_s(v_wnull, 10, L"abc", 3); POST(r, EINVAL);
                      PRE(); r = wmemcpy_s(d, v_two, L"abc", 3); POST(r, ERANGE); }
TC_CASE(wmemmove_s_){ wchar_t d[4] = { 0 }; int r; PRE(); r = wmemmove_s(v_wnull, 10, L"abc", 3); POST(r, EINVAL);
                      PRE(); r = wmemmove_s(d, v_two, L"abc", 3); POST(r, ERANGE); }
TC_CASE(wcsncpy_s_) { wchar_t d[4] = { 0 }; int r; PRE(); r = wcsncpy_s(v_wnull, 10, L"a", 1); POST(r, EINVAL);
                      PRE(); r = wcsncpy_s(d, v_two, L"abc", 3); POST(r, ERANGE); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no handler api\n"); return 1; }
    TC_RUN(wcscat_s_); TC_RUN(wmemcpy_s_); TC_RUN(wmemmove_s_); TC_RUN(wcsncpy_s_);
    return tc_end();
}
