/* TC
 * id:    ucr_neg_strtol_bad_base
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtol, strtoul (base must be 0 or 2..36; otherwise EINVAL and the invalid-parameter handler)
 * note:  endptr is not asserted here; see ucr_neg_strtol_bad_base_endptr_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;
static volatile int v_base;

static void badl(int base)
{
    char *e = NULL; long r; v_s = "123"; v_base = base; tc_iph_count = 0; errno = 0;
    r = strtol(v_s, &e, v_base);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
static void badul(int base)
{
    char *e = NULL; unsigned long r; v_s = "123"; v_base = base; tc_iph_count = 0; errno = 0;
    r = strtoul(v_s, &e, v_base);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(strtol_base1) { badl(1); }
TC_CASE(strtol_base37) { badl(37); }
TC_CASE(strtol_base_minus1) { badl(-1); }
TC_CASE(strtoul_base1) { badul(1); }
TC_CASE(strtoul_base37) { badul(37); }
TC_CASE(strtoul_base_minus1) { badul(-1); }
int main(void)
{
    tc_begin(); if (!tc_iph_install()) return 2;
    TC_RUN(strtol_base1); TC_RUN(strtol_base37); TC_RUN(strtol_base_minus1);
    TC_RUN(strtoul_base1); TC_RUN(strtoul_base37); TC_RUN(strtoul_base_minus1);
    return tc_end();
}
