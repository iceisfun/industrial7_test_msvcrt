/* TC
 * id:    scn_neg_sscanf_s_too_small_state_probe
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs scanf_s (not precise about post-failure state)
 * note:  PROBES post-failure state when sscanf_s("abcdef 7","%s%d",buf,4,&i) overflows with a returning handler:
 *        the invalid parameter handler runs once (MS docs say overflow is an invalid-parameter condition; not certain), buf[0]==0, errno==ERANGE, return==0, and i is not assigned. UCRT is believed to write an empty string and set ERANGE.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vin;
static volatile unsigned vsz;

static int run(char *b, int *i, int *err)
{
    int n; vin = "abcdef 7"; vsz = 4; *i = 99;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = sscanf_s(vin, "%s%d", b, vsz, i);
    *err = errno; return n;
}
TC_CASE(iph_once) { char b[8] = "ZZZZZZZ"; int i, e; run(b, &i, &e); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(buf_emptied) { char b[8] = "ZZZZZZZ"; int i, e; run(b, &i, &e); CHECK_EQ_I(b[0], 0); }
TC_CASE(errno_erange) { char b[8] = "ZZZZZZZ"; int i, e; run(b, &i, &e); CHECK_EQ_I(e, ERANGE); }
TC_CASE(ret_and_unassigned) { char b[8] = "ZZZZZZZ"; int i, e, n; n = run(b, &i, &e); CHECK_EQ_I(n, 0); CHECK_EQ_I(i, 99); }

int main(void) { tc_begin(); TC_RUN(iph_once); TC_RUN(buf_emptied); TC_RUN(errno_erange); TC_RUN(ret_and_unassigned); return tc_end(); }
