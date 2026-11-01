/* TC
 * id:    str_neg_default_exitcode_probe
 * area:  string
 * funcs: strcpy_s
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   ucrt invalid_parameter default path -> __fastfail(FAST_FAIL_INVALID_ARG)
 * note:  PROBE: exact process exit code when strcpy_s(NULL,..) hits the ucrt default invalid-parameter handler with Watson/WER suppressed
 *        (expected 0xC0000409 STATUS_STACK_BUFFER_OVERRUN). Child process.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>

static char *volatile pd;

TC_CASE(exit_code)
{
    DWORD code = 0; int ran = tc_spawn_self("defh", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); pd = NULL; strcpy_s(pd, 1, "a"); return 77; }
    TC_RUN(exit_code);
    return tc_end();
}
