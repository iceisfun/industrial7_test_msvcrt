/* TC
 * id:    str_neg_strcpy_s_fatal_default
 * area:  string
 * funcs: strcpy_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation" / _set_invalid_parameter_handler: with no handler installed the process is terminated
 * note:  Asserts only abnormal termination (exit code not 0 and not the 77 "returned" marker). Exact code: str_neg_default_exitcode_probe. Runs in a child process.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>

static char *volatile pd;

TC_CASE(default_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("defh", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("iph", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();
        if (c[0] == 'i') tc_iph_install();
        pd = NULL; strcpy_s(pd, 1, "a");
        return 77;
    }
    TC_RUN(default_terminates); TC_RUN(handler_returns);
    return tc_end();
}
