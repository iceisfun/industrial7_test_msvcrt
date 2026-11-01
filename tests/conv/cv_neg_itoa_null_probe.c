/* TC
 * id:    cv_neg_itoa_null_probe
 * area:  conv
 * funcs: _itoa _ltoa _ultoa _i64toa _ui64toa
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _itoa_s: NULL buffer -> EINVAL and invalid parameter handler
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice; class C): _itoa/_ltoa/_ultoa/_i64toa/_ui64toa given a NULL buffer do not validate it (the docs
 *        state validation only for the _s variants): the child dies with STATUS_ACCESS_VIOLATION (0xC0000005) before any handler or errno effect.
 *        The original probe expected handler once + EINVAL + NULL return. Each call runs in a child process and the exit code is asserted.
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
/* observed Win10 22H2 ucrtbase: the non-_s routines do not check the buffer; NULL is dereferenced -> access violation in the child */
static void run(const char *n) { DWORD code = 0; int ok = tc_spawn_self(n, &code, 20000); CHECK(ok); CHECK_EQ_U(code, 0xC0000005u); }

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
