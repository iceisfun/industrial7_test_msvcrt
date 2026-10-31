/* TC
 * id:    str_neg_strncpy_s_too_small
 * area:  string
 * funcs: 
 * procs: strncpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s (ERANGE when dest too small and count != _TRUNCATE)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pncpy_t)(char *, size_t, const char *, size_t);
static pncpy_t p_ncpy;

TC_CASE(erange_empty) { char d[4]; volatile const char *s = "abcdef"; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 6), ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(errno_iph) { char d[4]; volatile const char *s = "abcdef"; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 6), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(control_count_fits) { char d[4]; volatile const char *s = "abcdef"; tc_iph_count = 0; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 3), 0); CHECK_STR(d, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(count_eq_size) { char d[4]; volatile const char *s = "abcd"; tc_iph_count = 0; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, 4), ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p_ncpy = (pncpy_t)tc_crt_proc("strncpy_s");
    if (!p_ncpy) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(erange_empty); TC_RUN(errno_iph); TC_RUN(control_count_fits); TC_RUN(count_eq_size);
    return tc_end();
}
