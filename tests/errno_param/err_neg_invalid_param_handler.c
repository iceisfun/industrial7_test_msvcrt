/* TC
 * id:    err_neg_invalid_param_handler
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s, _set_invalid_parameter_handler, Parameter validation
 * note:  Negative-test exemplar (msvcr80 omitted only because mingw's libmsvcr80.a lacks strcpy_s; use procs + TC_DECL_PROC to cover it). A bad argument must (1) invoke the handler once, (2) return the documented error, (3) set errno.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(null_dest)
{
    char src[] = "abc";
    errno_t r; tc_iph_count = 0; errno = 0;
    r = strcpy_s(NULL, 10, src);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(buffer_too_small)
{
    char d[3] = { 'x', 'x', 'x' }; errno_t r; tc_iph_count = 0; errno = 0;
    r = strcpy_s(d, sizeof d, "abcdef");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);  /* documented: dest[0] set to 0 on failure */
}
TC_CASE(zero_size)
{
    char d[4] = "q"; errno_t r; tc_iph_count = 0;
    r = strcpy_s(d, 0, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_dest); TC_RUN(buffer_too_small); TC_RUN(zero_size);
    return tc_end();
}
