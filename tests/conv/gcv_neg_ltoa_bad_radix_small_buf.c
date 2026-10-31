/* TC
 * id:    gcv_neg_ltoa_bad_radix_small_buf
 * area:  conv
 * funcs: _ltoa
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ltoa (no documented behaviour for radix outside 2..36 in the non-_s function)
 * note:  ucrtbase only: legacy CRTs divide by zero (radix 0) or loop (radix 1) in the non-_s routine. PROBING: _ltoa with radix 0, 1, 37 does not crash, never writes beyond a 34-byte buffer, and (when it returns)
 *        the guard bytes after buf[33] are intact. On CRTs with a fatal default invalid-parameter handler each call runs
 *        in a child; child exit 10 = returned and guard intact, 11 = guard damaged. Return value/buffer contents are not asserted.
 *        Pre-existing cv_neg_itoa_bad_radix_probe asserts buf[0]==0/EINVAL (ucrtbase, radix 40) - not duplicated here.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int rad;

static int child_main(const char *n)
{
    struct { char b[34]; char guard[16]; } g; unsigned i;
    for (i = 0; i < sizeof g.b; i++) g.b[i] = 'X';
    for (i = 0; i < sizeof g.guard; i++) g.guard[i] = (char)0xA5;
    tc_iph_install();
    if (tc_strequ(n, "r0")) rad = 0; else if (tc_strequ(n, "r1")) rad = 1; else if (tc_strequ(n, "r37")) rad = 37; else return 13;
    _ltoa(5, g.b, rad);
    for (i = 0; i < sizeof g.guard; i++) if ((unsigned char)g.guard[i] != 0xA5) return 11;
    return 10;
}
static void run(const char *n)
{
    DWORD code = 0; char b[24]; int ok = tc_spawn_self(n, &code, 20000);
    tc_hex(code, b); tc_write("TC NOTE " TC_TEST_ID "::"); tc_write(n); tc_write(" exit="); tc_write(b); tc_write("\n");
    CHECK(ok); CHECK_EQ_U(code, 10);
}
TC_CASE(r0) { run("r0"); }
TC_CASE(r1) { run("r1"); }
TC_CASE(r37) { run("r37"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(r0); TC_RUN(r1); TC_RUN(r37);
    return tc_end();
}
