/* TC
 * id:    gcv_neg_imaxdiv_zero_child
 * area:  conv
 * funcs: imaxdiv
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C99 7.8.2.2 (undefined when result not representable)
 * note:  PROBING: imaxdiv(1,0) and imaxdiv(INTMAX_MIN,-1) trap on x64 (idiv). Only asserts the child does not return
 *        normally (exit code != 77); the exact status code is printed as a TC NOTE, not asserted.
 */
#include "tc.h"
#include <inttypes.h>
#include <stdint.h>

static volatile intmax_t n, d;

static int child_main(const char *name)
{
    imaxdiv_t r;
    if (tc_strequ(name, "div0")) { n = 1; d = 0; }
    else if (tc_strequ(name, "ovf")) { n = INTMAX_MIN; d = -1; }
    else return 13;
    r = imaxdiv(n, d);
    return (int)(r.quot & 0) + 77;
}
static void run(const char *name)
{
    DWORD code = 0; char b[24]; int ok = tc_spawn_self(name, &code, 20000);
    tc_hex(code, b); tc_write("TC NOTE " TC_TEST_ID "::"); tc_write(name); tc_write(" exit="); tc_write(b); tc_write("\n");
    CHECK(ok);
    CHECK(code != 77);
}
TC_CASE(div0) { run("div0"); }
TC_CASE(ovf) { run("ovf"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(div0); TC_RUN(ovf);
    return tc_end();
}
