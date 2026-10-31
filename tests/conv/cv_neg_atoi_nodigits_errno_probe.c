/* TC
 * id:    cv_neg_atoi_nodigits_errno_probe
 * area:  conv
 * funcs: atoi atol _atoi64
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atoi (documents ERANGE only)
 * note:  PROBE: errno is left untouched (stays 0) when the string has no digits; no EINVAL is set and no invalid-parameter call occurs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile in;
TC_CASE(errno_zero)
{
    const char *v[] = { "abc", "", "   ", "-", "+" }; int i;
    for (i = 0; i < 5; i++) {
        in = v[i];
        errno = 0; CHECK_EQ_I(atoi(in), 0); CHECK_EQ_I(errno, 0);
        errno = 0; CHECK_EQ_I(atol(in), 0); CHECK_EQ_I(errno, 0);
        errno = 0; CHECK_EQ_I(_atoi64(in), 0); CHECK_EQ_I(errno, 0);
    }
}
TC_CASE(no_iph)
{
    int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    n = tc_iph_count; in = "abc"; (void)atoi(in); CHECK_EQ_I(tc_iph_count, n);
}
int main(void) { tc_begin(); TC_RUN(errno_zero); TC_RUN(no_iph); return tc_end(); }
