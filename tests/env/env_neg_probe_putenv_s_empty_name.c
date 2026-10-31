/* TC
 * id:    env_neg_probe_putenv_s_empty_name
 * area:  env
 * kind:  negative
 * funcs:
 * procs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _putenv_s (empty varname not tabulated)
 * note:  PROBE: _putenv_s("", "v") is rejected with return EINVAL and errno EINVAL. Handler count not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, _putenv_s, (const char *, const char *));
static const char *volatile nm_empty = "";
static const char *volatile val = "v";

TC_CASE(empty_name)
{
    int r;
    if (!tc_p__putenv_s) TC_SKIP("not exported");
    errno = 0;
    r = tc_p__putenv_s(nm_empty, val);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_putenv_s);
    TC_RUN(empty_name);
    return tc_end();
}
