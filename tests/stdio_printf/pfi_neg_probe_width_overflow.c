/* TC
 * id:    pfi_neg_probe_width_overflow
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification, Parameter Validation
 * note:  PROBE: ucrt handling of a width/precision whose digits overflow int. Run in a child process; assert only that
 *        it terminates without crashing and either _scprintf returns -1 or the invalid-parameter handler ran.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_w = "%2147483648d";
static const char * volatile f_p = "%.2147483648d";

/* Child process: if the CRT instead treats the overflowed number as a huge/negative width it may spin padding for
 * a very long time or crash; isolating it keeps that from taking down the parent. */
static int child_main(const char *name)
{
    int n;
    tc_iph_install(); tc_iph_count = 0;
    n = _scprintf(name[0] == 'w' ? f_w : f_p, 1);
    return (n == -1 || tc_iph_count > 0) ? 0 : 1;
}
static void run(const char *which)
{
    DWORD code = 0xffff; int ok = tc_spawn_self(which, &code, 60000);
    CHECK(ok);          /* terminated within the timeout */
    CHECK(code != TC_STATUS_ACCESS_VIOLATION);
    CHECK_EQ_U(code, 0); /* -1 or invalid-parameter handler invoked */
}
TC_CASE(width)     { run("width"); }
TC_CASE(precision) { run("precision"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(width); TC_RUN(precision);
    return tc_end();
}
