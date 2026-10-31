/* TC
 * id:    cv_neg_strtod_nodigits_errno_probe
 * area:  conv
 * funcs: atof
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3; MS docs strtod
 * note:  Probes (1) whether errno is left untouched when strtod finds no digits (sentinel 1234; C allows EINVAL, MS docs
 *        do not mention setting it) and (2) that "-abc"/"-" yield +0.0 not -0.0 (no conversion performed, so no negation).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(double, strtod, (const char *, char **));
static const char *volatile vs;

TC_CASE(errno_unchanged)
{
    char *e;
    errno = 1234; vs = "abc"; (void)tc_p_strtod(vs, &e); CHECK_EQ_I(errno, 1234);
    errno = 1234; vs = "";    (void)tc_p_strtod(vs, &e); CHECK_EQ_I(errno, 1234);
    errno = 1234; vs = "abc"; (void)atof(vs);       CHECK_EQ_I(errno, 1234);
}
TC_CASE(neg_no_digits_is_positive_zero)
{
    char *e; double d;
    vs = "-abc"; d = tc_p_strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vs);
    vs = "-";    d = tc_p_strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vs);
}
int main(void) { tc_begin(); TC_LOAD_PROC(strtod); TC_RUN(errno_unchanged); TC_RUN(neg_no_digits_is_positive_zero); return tc_end(); }
