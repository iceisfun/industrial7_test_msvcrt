/* TC
 * id:    cv_neg_strtol_bad_base
 * area:  conv
 * funcs: strtol strtoul _strtoi64
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4 (base must be 0 or 2..36); MS docs Parameter validation
 * note:  Probes that bases 1, 37 and -1 are rejected by the secure-CRT validation: return 0, errno EINVAL, invalid-parameter handler called once per call (not a crash, no conversion).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile vs = "123";

TC_CASE(base_1_37)
{
    int n0 = tc_iph_count;
    errno = 0; CHECK_EQ_I(strtol((const char *)vs, NULL, 1), 0);  CHECK_EQ_I(errno, 22);
    errno = 0; CHECK_EQ_I(strtol((const char *)vs, NULL, 37), 0); CHECK_EQ_I(errno, 22);
    errno = 0; CHECK_EQ_I(strtol((const char *)vs, NULL, -1), 0); CHECK_EQ_I(errno, 22);
    CHECK_EQ_I(tc_iph_count - n0, 3);
}
TC_CASE(unsigned_and_64)
{
    int n0 = tc_iph_count;
    errno = 0; CHECK_EQ_U(strtoul((const char *)vs, NULL, 37), 0); CHECK_EQ_I(errno, 22);
    errno = 0; CHECK_EQ_I(_strtoi64((const char *)vs, NULL, 1), 0); CHECK_EQ_I(errno, 22);
    CHECK_EQ_I(tc_iph_count - n0, 2);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(base_1_37); TC_RUN(unsigned_and_64);
    return tc_end();
}
