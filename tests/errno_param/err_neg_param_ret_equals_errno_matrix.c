/* TC
 * id:    err_neg_param_ret_equals_errno_matrix
 * area:  errno_param
 * kind:  negative
 * funcs: strcat_s strncat_s memmove_s strncpy_s _strlwr_s _strupr_s _set_invalid_parameter_handler
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

TC_CASE(strcat_s_)  { char d[4] = ""; int r; PRE(); r = strcat_s(v_null, 10, "a"); POST(r, EINVAL);
                      PRE(); r = strcat_s(d, v_two, "abc"); POST(r, ERANGE); }
TC_CASE(strncat_s_) { char d[4] = ""; int r; PRE(); r = strncat_s(v_null, 10, "a", 1); POST(r, EINVAL);
                      PRE(); r = strncat_s(d, v_two, "abc", 3); POST(r, ERANGE); }
TC_CASE(memmove_s_) { char d[4] = ""; int r; PRE(); r = memmove_s(v_null, 10, "abc", 3); POST(r, EINVAL);
                      PRE(); r = memmove_s(d, v_two, "abc", 3); POST(r, ERANGE); }
TC_CASE(strncpy_s_) { char d[4] = ""; int r; PRE(); r = strncpy_s(v_null, 10, "a", 1); POST(r, EINVAL);
                      PRE(); r = strncpy_s(d, v_two, "abc", 3); POST(r, ERANGE); }
TC_CASE(strlwr_s_)  { char d[3] = { 'A', 'B', 'C' }; int r; PRE(); r = _strlwr_s(v_null, 10); POST(r, EINVAL);
                      PRE(); r = _strlwr_s(d, 3); POST(r, EINVAL); /* not NUL-terminated within size */ }
TC_CASE(strupr_s_)  { char d[3] = { 'a', 'b', 'c' }; int r; PRE(); r = _strupr_s(v_null, 10); POST(r, EINVAL);
                      PRE(); r = _strupr_s(d, 3); POST(r, EINVAL); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no handler api\n"); return 1; }
    TC_RUN(strcat_s_); TC_RUN(strncat_s_); TC_RUN(memmove_s_); TC_RUN(strncpy_s_); TC_RUN(strlwr_s_); TC_RUN(strupr_s_);
    return tc_end();
}
