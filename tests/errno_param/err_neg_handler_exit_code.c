/* TC
 * id:    err_neg_handler_exit_code
 * area:  errno_param
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, strcpy_s, Parameter validation
 * note:  An installed handler may terminate the process itself; if it returns, strcpy_s returns EINVAL. Both paths run in child processes.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static void __cdecl exit_handler(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; ExitProcess(123); }
static void __cdecl ret_handler(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; }

TC_CASE(handler_exits)
{
    DWORD code = 0; int ran = tc_spawn_self("exit", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 123);
}
TC_CASE(handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char d[4]; errno_t r;
        if (c[0] == 'e') { _set_invalid_parameter_handler(exit_handler); strcpy_s(NULL, 10, "x"); return 77; }
        _set_invalid_parameter_handler(ret_handler);
        r = strcpy_s(NULL, 10, "x");
        (void)d;
        return r == EINVAL ? 77 : 1;
    }
    TC_RUN(handler_exits); TC_RUN(handler_returns);
    return tc_end();
}
