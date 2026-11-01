/* TC
 * id:    cv_neg_itoa_bad_radix_probe
 * area:  conv
 * funcs: _itoa _ltoa _ultoa _i64toa
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _itoa_s: radix outside [2,36] -> EINVAL, invalid parameter handler, buffer[0] = 0
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice; class C for radix 0/1): the non-_s routines do NOT validate the radix (the docs state
 *        validation only for the _s variants). _itoa radix 0 -> divide by zero (0xC0000094); radix 1 -> endless digit loop ending in an access violation
 *        (0xC0000005); radix 37/40/100 -> returned normally, buffer "5", errno unchanged, invalid-parameter handler NOT called (child exit 10).
 *        The original guess (empty string + EINVAL) was wrong. Each call runs in a child process.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int rad;

static int child_main(const char *n)
{
    char b[80] = "XXXX";
    if (!tc_iph_install()) return 12;
    errno = 0; tc_iph_count = 0;
    if (tc_strequ(n, "itoa_1")) { rad = 1; _itoa(5, b, rad); }
    else if (tc_strequ(n, "itoa_37")) { rad = 37; _itoa(5, b, rad); }
    else if (tc_strequ(n, "itoa_0")) { rad = 0; _itoa(5, b, rad); }
    else if (tc_strequ(n, "ltoa_40")) { rad = 40; _ltoa(5, b, rad); }
    else if (tc_strequ(n, "ultoa_37")) { rad = 37; _ultoa(5, b, rad); }
    else if (tc_strequ(n, "i64_100")) { rad = 100; _i64toa(5, b, rad); }
    else return 13;
    /* radix >= 37 returned: observed result is the plain digit "5", errno untouched, handler NOT called */
    return (b[0] == '5' && b[1] == 0 && errno == 0 && tc_iph_count == 0) ? 10 : 11;
}
static void run(const char *n, DWORD want) { DWORD code = 0; int ok = tc_spawn_self(n, &code, 20000); CHECK(ok); CHECK_EQ_U(code, want); }

/* observed Win10 22H2 ucrtbase: the non-_s routines do not validate the radix. 0 -> divide by zero, 1 -> runaway digit loop ending in an
 * access violation, 37/40/100 -> accepted, digit 5 converts normally. */
TC_CASE(itoa_1) { run("itoa_1", 0xC0000005u); }
TC_CASE(itoa_37) { run("itoa_37", 10); }
TC_CASE(itoa_0) { run("itoa_0", 0xC0000094u); }
TC_CASE(ltoa_40) { run("ltoa_40", 10); }
TC_CASE(ultoa_37) { run("ultoa_37", 10); }
TC_CASE(i64_100) { run("i64_100", 10); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(itoa_1); TC_RUN(itoa_37); TC_RUN(itoa_0); TC_RUN(ltoa_40); TC_RUN(ultoa_37); TC_RUN(i64_100);
    return tc_end();
}
