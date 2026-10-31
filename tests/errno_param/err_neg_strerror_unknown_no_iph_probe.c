/* TC
 * id:    err_neg_strerror_unknown_no_iph_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror
 * note:  PROBE: strerror with an out-of-range errnum does NOT invoke the invalid-parameter handler (it returns the unknown-error text). Not stated in the docs.
 */
#include "tc.h"
#include <string.h>

static volatile int v_e;

TC_CASE(no_handler)
{
    tc_iph_count = 0;
    v_e = 9999; (void)strerror(v_e);
    v_e = -1;   (void)strerror(v_e);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(no_handler); return tc_end(); }
