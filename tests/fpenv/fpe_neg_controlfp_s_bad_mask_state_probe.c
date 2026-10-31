/* TC
 * id:    fpe_neg_controlfp_s_bad_mask_state_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _controlfp_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _controlfp_s
 * note:  Probes (1) whether any control-word bits are applied when _controlfp_s rejects an undefined mask bit, (2) what *currentControl holds afterwards (untouched sentinel, or the current control word). Asserted: word unchanged; *current is sentinel or current word. Actual value is logged as TC NOTE.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));
static void log_hex(const char *tag, unsigned int v) { char b[24]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(tag); tc_hex(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(state_after_failure)
{
    unsigned int cur = 0xdeadbeef, before, after; errno_t r;
    TC_LOAD_PROC(_controlfp_s);
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    before = _controlfp(0, 0);
    /* undefined bit 14 in mask, plus a real change request (RC) that must not be applied if the call is rejected */
    r = tc_p__controlfp_s(&cur, 0x00004000 | _RC_UP, 0x00004000 | _MCW_RC);
    after = _controlfp(0, 0);
    log_hex("ret=", (unsigned)r); log_hex("cur=", cur); log_hex("before=", before); log_hex("after=", after);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_U(after, before);
    CHECK(cur == 0xdeadbeef || cur == before);
    _controlfp(before, _MCW_RC);
}
int main(void) { tc_begin(); TC_RUN(state_after_failure); return tc_end(); }
