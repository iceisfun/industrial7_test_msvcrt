/* TC
 * id:    fpe_neg_controlfp_s_pc_mask_x64_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _controlfp_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _controlfp_s (_MCW_PC not supported on x64/ARM)
 * note:  Probes how _controlfp_s treats _MCW_PC on x64: EINVAL with invalid-parameter handler, or silently ignored (0). Asserted only: precision bits unchanged and ret is 0 or EINVAL; return value and iph count are logged as TC NOTE.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));
static void log_hex(const char *tag, unsigned int v) { char b[24]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(tag); tc_hex(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(pc_ignored_or_rejected)
{
#ifdef _WIN64
    unsigned int cur = 0, before, after; errno_t r;
    TC_LOAD_PROC(_controlfp_s);
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    before = _controlfp(0, 0);
    r = tc_p__controlfp_s(&cur, _PC_24, _MCW_PC);
    after = _controlfp(0, 0);
    log_hex("ret=", (unsigned)r); log_hex("iph=", (unsigned)tc_iph_count); log_hex("cur=", cur);
    CHECK(r == 0 || r == EINVAL);
    CHECK_EQ_U(after & _MCW_PC, before & _MCW_PC);
    CHECK_EQ_U(after, before);
    if (r == EINVAL) CHECK_EQ_I(tc_iph_count, 1);
#else
    TC_SKIP("x64 only");
#endif
}
int main(void) { tc_begin(); TC_RUN(pc_ignored_or_rejected); return tc_end(); }
