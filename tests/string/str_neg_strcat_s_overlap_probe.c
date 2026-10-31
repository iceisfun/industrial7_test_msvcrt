/* TC
 * id:    str_neg_strcat_s_overlap_probe
 * area:  string
 * funcs: 
 * procs: strcat_s
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcat_s Remarks (overlap behaviour undefined)
 * note:  PROBE: strcat_s(buf, n, buf) with src==dest. Docs say undefined. Most likely ucrt keeps appending
 *        its own growing output until the buffer is exhausted, then reports ERANGE (or EINVAL) and sets buf[0]=0.
 *        Asserts only: returns non-zero, buf[0]==0, no crash/hang.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
typedef int (__cdecl *pcat_t)(char *, size_t, const char *);
TC_CASE(overlap_self)
{
    pcat_t p = (pcat_t)tc_crt_proc("strcat_s"); char b[16]; volatile char *src = b; int r;
    if (!p) TC_SKIP("strcat_s not exported");
    strcpy(b, "ab"); tc_iph_count = 0;
    r = p(b, sizeof b, (const char *)src);
    CHECK(r == ERANGE || r == EINVAL); CHECK_EQ_I(b[0], 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(overlap_self); return tc_end(); }
