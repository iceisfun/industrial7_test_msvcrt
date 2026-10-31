/* TC
 * id:    cv_strtol_radix_range
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4p3

 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;
static const char dig[] = "0123456789abcdefghijklmnopqrstuvwxyz";

TC_CASE(zz36) { char *e; p = "zz"; CHECK_EQ_I(strtol(p, &e, 36), 1295); CHECK_EQ_P(e, p + 2); p = "ZZ"; CHECK_EQ_I(strtol(p, &e, 36), 1295); CHECK_EQ_P(e, p + 2); }
TC_CASE(mixed_case) { char *e; p = "Zz"; CHECK_EQ_I(strtol(p, &e, 36), 1295); CHECK_EQ_P(e, p + 2); p = "zZ"; CHECK_EQ_I(strtol(p, &e, 36), 1295); }
TC_CASE(base11) { char *e; p = "a"; CHECK_EQ_I(strtol(p, &e, 11), 10); CHECK_EQ_P(e, p + 1); p = "A"; CHECK_EQ_I(strtol(p, &e, 11), 10); p = "b"; e = (char *)1; CHECK_EQ_I(strtol(p, &e, 11), 0); CHECK_EQ_P(e, p); }
TC_CASE(base35_z) { char *e = (char *)1; p = "z"; CHECK_EQ_I(strtol(p, &e, 35), 0); CHECK_EQ_P(e, p); p = "y"; CHECK_EQ_I(strtol(p, &e, 35), 34); CHECK_EQ_P(e, p + 1); }
TC_CASE(each_radix)
{
    int r;
    for (r = 2; r <= 36; r++) {
        char buf[4]; char *e; long v;
        buf[0] = dig[r - 1]; buf[1] = dig[r < 36 ? r : 0]; buf[2] = 0;
        if (r == 36) buf[1] = '!';
        p = buf;
        v = strtol(p, &e, r);
        if (v != r - 1 || e != buf + 1) { CHECK_EQ_I(r, -1); CHECK_EQ_I(v, r - 1); CHECK_EQ_P(e, buf + 1); }
        buf[0] = (char)(dig[r - 1] >= 'a' ? dig[r - 1] - 32 : dig[r - 1]);
        v = strtol(p, &e, r);
        if (v != r - 1 || e != buf + 1) { CHECK_EQ_I(r, -2); CHECK_EQ_I(v, r - 1); CHECK_EQ_P(e, buf + 1); }
    }
}

int main(void) { tc_begin(); TC_RUN(zz36); TC_RUN(mixed_case); TC_RUN(base11); TC_RUN(base35_z); TC_RUN(each_radix); return tc_end(); }
