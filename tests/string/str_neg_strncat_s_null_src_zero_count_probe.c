/* TC
 * id:    str_neg_strncat_s_null_src_zero_count_probe
 * area:  string
 * funcs: 
 * procs: strncat_s
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncat_s
 * note:  PROBE: strncat_s(buf,n,NULL,0). Believed to return 0 (count==0 is checked before src validation in ucrt);
 *        not stated in the docs. Also probes that no invalid-parameter call happens.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pnc_t)(char *, size_t, const char *, size_t);
TC_CASE(null_src_count0)
{
    pnc_t p = (pnc_t)tc_crt_proc("strncat_s"); char d[8]; volatile const char *s = NULL;
    if (!p) TC_SKIP("strncat_s not exported");
    strcpy(d, "ab"); tc_iph_count = 0;
    CHECK_EQ_I(p(d, sizeof d, (const char *)s, 0), 0); CHECK_STR(d, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_src_count0); return tc_end(); }
