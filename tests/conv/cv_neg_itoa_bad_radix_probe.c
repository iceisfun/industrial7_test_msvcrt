/* TC
 * id:    cv_neg_itoa_bad_radix_probe
 * area:  conv
 * funcs: _itoa _ltoa _ultoa _i64toa
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s: radix outside [2,36] -> EINVAL, invalid parameter handler, buffer[0] = 0
 * note:  PROBING: the non-_s routines with a bad radix (0, 1, 37, 40, 100) leave an empty string in the buffer
 *        (the docs only state this for the _s variants). Each call runs in a child process (a crash must not mask other cases);
 *        child exits 10 iff buf[0]==0 and errno==EINVAL.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static volatile int rad;

static int child_main(const char *n)
{
    char b[80] = "XXXX";
    if (!tc_iph_install()) return 12;
    errno = 0;
    if (tc_strequ(n, "itoa_1")) { rad = 1; _itoa(5, b, rad); }
    else if (tc_strequ(n, "itoa_37")) { rad = 37; _itoa(5, b, rad); }
    else if (tc_strequ(n, "itoa_0")) { rad = 0; _itoa(5, b, rad); }
    else if (tc_strequ(n, "ltoa_40")) { rad = 40; _ltoa(5, b, rad); }
    else if (tc_strequ(n, "ultoa_37")) { rad = 37; _ultoa(5, b, rad); }
    else if (tc_strequ(n, "i64_100")) { rad = 100; _i64toa(5, b, rad); }
    else return 13;
    return (b[0] == 0 && errno == EINVAL) ? 10 : 11;
}
static void run(const char *n) { DWORD code = 0; int ok = tc_spawn_self(n, &code, 20000); CHECK(ok); CHECK_EQ_U(code, 10); }

TC_CASE(itoa_1) { run("itoa_1"); }
TC_CASE(itoa_37) { run("itoa_37"); }
TC_CASE(itoa_0) { run("itoa_0"); }
TC_CASE(ltoa_40) { run("ltoa_40"); }
TC_CASE(ultoa_37) { run("ultoa_37"); }
TC_CASE(i64_100) { run("i64_100"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(itoa_1); TC_RUN(itoa_37); TC_RUN(itoa_0); TC_RUN(ltoa_40); TC_RUN(ultoa_37); TC_RUN(i64_100);
    return tc_end();
}
