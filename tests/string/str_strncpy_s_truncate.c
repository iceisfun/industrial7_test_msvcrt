/* TC
 * id:    str_strncpy_s_truncate
 * area:  string
 * funcs: 
 * procs: strncpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strncpy_s (_TRUNCATE, STRUNCATE=80)
 * note:  _TRUNCATE is (size_t)-1, STRUNCATE is 80; errno/handler behaviour on truncation is probed separately in str_strn_s_truncate_quiet_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pncpy_t)(char *, size_t, const char *, size_t);
static pncpy_t p_ncpy;
#define TRUNC ((size_t)-1)

TC_CASE(short_buffer) { char d[4]; volatile const char *s = "abcdef"; memset(d, 'X', sizeof d); CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, TRUNC), 80); CHECK_STR(d, "abc"); CHECK_EQ_I(d[3], 0); }
TC_CASE(fits) { char d[8]; volatile const char *s = "abc"; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, TRUNC), 0); CHECK_STR(d, "abc"); }
TC_CASE(exact_fit) { char d[4]; volatile const char *s = "abc"; CHECK_EQ_I(p_ncpy(d, sizeof d, (const char *)s, TRUNC), 0); CHECK_STR(d, "abc"); }
TC_CASE(size_one) { char d[1]; volatile const char *s = "abc"; d[0] = 'X'; CHECK_EQ_I(p_ncpy(d, 1, (const char *)s, TRUNC), 80); CHECK_EQ_I(d[0], 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    p_ncpy = (pncpy_t)tc_crt_proc("strncpy_s");
    if (!p_ncpy) { tc_write("TC FAIL " TC_TEST_ID "::__import strncpy_s missing\n"); return 1; }
    TC_RUN(short_buffer); TC_RUN(fits); TC_RUN(exact_fit); TC_RUN(size_one);
    return tc_end();
}
