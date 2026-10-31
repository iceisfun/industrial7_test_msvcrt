/* TC
 * id:    cv_neg_itoa_null_probe
 * area:  conv
 * funcs: _itoa _ltoa _ultoa _i64toa _ui64toa
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s: NULL buffer -> EINVAL and invalid parameter handler
 * note:  PROBING: that the non-_s routines given a NULL buffer invoke the invalid parameter handler once, set errno to EINVAL
 *        and return NULL (docs state this only for the _s variants). Each call runs in a child process (a crash must not mask other cases); child exits 10 iff errno==EINVAL and iph count==1
 *        (and, for _itoa only, return == NULL).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char *volatile nb;
static volatile int rad = 10;

static int child_main(const char *n)
{
    int ret_null = 1;
    if (!tc_iph_install()) return 12;
    errno = 0; tc_iph_count = 0;
    if (tc_strequ(n, "itoa")) ret_null = (_itoa(1, nb, rad) == NULL);
    else if (tc_strequ(n, "ltoa")) _ltoa(1, nb, rad);
    else if (tc_strequ(n, "ultoa")) _ultoa(1, nb, rad);
    else if (tc_strequ(n, "i64toa")) _i64toa(1, nb, rad);
    else if (tc_strequ(n, "ui64toa")) _ui64toa(1, nb, rad);
    else return 13;
    return (errno == EINVAL && tc_iph_count == 1 && ret_null) ? 10 : 11;
}
static void run(const char *n) { DWORD code = 0; int ok = tc_spawn_self(n, &code, 20000); CHECK(ok); CHECK_EQ_U(code, 10); }

TC_CASE(itoa) { run("itoa"); }
TC_CASE(ltoa) { run("ltoa"); }
TC_CASE(ultoa) { run("ultoa"); }
TC_CASE(i64toa) { run("i64toa"); }
TC_CASE(ui64toa) { run("ui64toa"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(itoa); TC_RUN(ltoa); TC_RUN(ultoa); TC_RUN(i64toa); TC_RUN(ui64toa);
    return tc_end();
}
