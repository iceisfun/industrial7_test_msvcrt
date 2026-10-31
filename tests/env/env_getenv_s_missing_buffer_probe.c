/* TC
 * id:    env_getenv_s_missing_buffer_probe
 * area:  env
 * procs: getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs getenv_s
 * note:  Probes: when the variable is not found, getenv_s sets buffer[0] to NUL (buffer is pre-filled with 'z').
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, getenv_s, (size_t *, char *, size_t, const char *));

TC_CASE(missing_sets_buffer_empty)
{
    size_t n = 99; char b[8] = "zzzzzzz"; int r;
    if (!tc_p_getenv_s) TC_SKIP("getenv_s not exported");
    r = tc_p_getenv_s(&n, b, sizeof b, "TCV_GS_SURELY_MISSING_6");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 0); CHECK(b[0] == 0);
}

int main(void) { tc_begin(); TC_LOAD_PROC(getenv_s); TC_RUN(missing_sets_buffer_empty); return tc_end(); }
