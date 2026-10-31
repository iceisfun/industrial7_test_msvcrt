/* TC
 * id:    err_neg_param_strncpy_s_truncate
 * area:  errno_param
 * kind:  negative
 * funcs: strncpy_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strncpy_s (STRUNCATE, ERANGE), _TRUNCATE
 * note:  msvcr80/90 omitted: mingw import libs lack strncpy_s.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#ifndef STRUNCATE
#define STRUNCATE 80
#endif

TC_CASE(truncate_small)
{
    char d[4]; errno_t r; volatile size_t cnt = _TRUNCATE;
    memset(d, 'x', sizeof d); tc_iph_count = 0;
    r = strncpy_s(d, sizeof d, "abcdefgh", cnt);
    CHECK_EQ_I(r, STRUNCATE);
    CHECK_STR(d, "abc");
    CHECK_EQ_I(d[3], 0);
    CHECK_EQ_I(tc_iph_count, 0);   /* documented: _TRUNCATE truncation does not invoke the handler */
}
TC_CASE(count_ge_size)
{
    char d[4]; errno_t r; volatile size_t cnt = 8;
    memset(d, 'x', sizeof d); tc_iph_count = 0; errno = 0;
    r = strncpy_s(d, sizeof d, "abcdefgh", cnt);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(fits_exactly)
{
    char d[4]; errno_t r; volatile size_t cnt = _TRUNCATE;
    tc_iph_count = 0;
    r = strncpy_s(d, sizeof d, "abc", cnt);
    CHECK_EQ_I(r, 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(truncate_small); TC_RUN(count_ge_size); TC_RUN(fits_exactly);
    return tc_end();
}
