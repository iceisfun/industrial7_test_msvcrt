/* TC
 * id:    pfi_neg_probe_sprintf_null_msvcrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification, Parameter Validation
 * note:  PROBE: msvcrt.dll has no invalid-parameter machinery for sprintf(NULL,...). The child either crashes
 *        (access violation) or returns (-1). Parent asserts only that the child terminated within the timeout.
 */
#include "tc.h"
#include <stdio.h>

static char * volatile nullbuf = NULL;

static int child_main(const char *name)
{
    int n; (void)name;
    n = sprintf(nullbuf, "%d", 1);
    return n == -1 ? 78 : 77;
}
TC_CASE(null_buffer_child)
{
    DWORD code = 0; int ok = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ok);
    CHECK(code != 0xdead0001 && code != 0xdead0002);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(null_buffer_child);
    return tc_end();
}
