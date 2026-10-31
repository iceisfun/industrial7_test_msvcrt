/* TC
 * id:    fpe_neg_controlfp_s_bad_mask_iph
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _controlfp_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _controlfp_s (Return value: EINVAL; invalid parameter handler)
 * note:  Mask bit 0x4000 is not part of any _MCW_* field. msvcrt.dll (Vista+) is included on the assumption it shares the VS2005 validation; unverified.
 *        _controlfp_s is resolved at run time because mingw import libs lack it for some CRTs. Whether the control word stays unchanged is probed separately.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));
static void log_hex(const char *tag, unsigned int v) { char b[24]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(tag); tc_hex(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(undefined_bit)
{
    unsigned int cur = 0xdeadbeef, before; errno_t r; int have;
    TC_LOAD_PROC(_controlfp_s);
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    have = tc_iph_install(); tc_iph_count = 0;
    if (!have) TC_SKIP("no _set_invalid_parameter_handler: bad call would hit default handler");
    before = _controlfp(0, 0);
    r = tc_p__controlfp_s(&cur, 0x00004000, 0x00004000);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _controlfp(before, _MCW_EM | _MCW_RC);   /* restore only what could have been touched */
}
TC_CASE(valid_call_no_iph)
{
    unsigned int cur = 0, before; errno_t r; int have;
    TC_LOAD_PROC(_controlfp_s);
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    have = tc_iph_install(); tc_iph_count = 0;
    before = _controlfp(0, 0);
    r = tc_p__controlfp_s(&cur, 0, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(cur, before);
    if (have) CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(undefined_bit); TC_RUN(valid_call_no_iph); return tc_end(); }
