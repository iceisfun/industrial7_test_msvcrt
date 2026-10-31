/* TC
 * id:    str_strn_s_truncate_quiet_probe
 * area:  string
 * funcs: 
 * procs: strncpy_s strncat_s
 * crt:   >=100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strncpy_s, strncat_s (_TRUNCATE / STRUNCATE)
 * note:  PROBE: STRUNCATE is returned without touching errno and without calling the invalid-parameter handler
 *        (believed from CRT behaviour; docs only say STRUNCATE is returned). Also probes the special case
 *        strncpy_s(NULL, 0, src, 0) == 0 with no handler call (docs table lists dest NULL as EINVAL generally).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pn_t)(char *, size_t, const char *, size_t);

TC_CASE(strncpy_s_trunc_quiet)
{
    pn_t p = (pn_t)tc_crt_proc("strncpy_s"); char d[4]; volatile const char *s = "abcdef";
    if (!p) TC_SKIP("strncpy_s not exported");
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p(d, sizeof d, (const char *)s, (size_t)-1), 80); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(strncat_s_trunc_quiet)
{
    pn_t p = (pn_t)tc_crt_proc("strncat_s"); char d[8]; volatile const char *s = "defghijk";
    if (!p) TC_SKIP("strncat_s not exported");
    strcpy(d, "abc"); errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p(d, sizeof d, (const char *)s, (size_t)-1), 80); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(strncpy_s_null_zero_zero)
{
    pn_t p = (pn_t)tc_crt_proc("strncpy_s"); volatile const char *s = "abc";
    if (!p) TC_SKIP("strncpy_s not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p(NULL, 0, (const char *)s, 0), 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(strncpy_s_trunc_quiet); TC_RUN(strncat_s_trunc_quiet); TC_RUN(strncpy_s_null_zero_zero);
    return tc_end();
}
