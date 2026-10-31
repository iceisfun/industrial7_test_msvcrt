/* TC
 * id:    ext_probe_neg_invalid_parameter_noinfo_noreturn
 * area:  exit_signal
 * kind:  negative
 * funcs: _invalid_parameter_noinfo_noreturn _set_invalid_parameter_handler _exit
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation"; ucrt <corecrt.h> declares _invalid_parameter_noinfo_noreturn as __declspec(noreturn)
 * note:  Probes (1) the handler runs and can _exit with a chosen code, (2) if the handler returns, the process still dies
 *        (exit code neither 0 nor the 77 sentinel; on ucrtbase expected 0xC0000409 - not asserted exactly because the older CRTs differ).
 *        Both run in child processes.
 */
#include "tc.h"
#include <stdlib.h>

static void __cdecl h_exit(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e;(void)f;(void)fl;(void)l;(void)r; _exit(66); }
static void __cdecl h_ret(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e;(void)f;(void)fl;(void)l;(void)r; }
static _invalid_parameter_handler volatile vexit = h_exit, vret = h_ret;

TC_CASE(handler_exits_66)
{
    DWORD code = 0; int ran = tc_spawn_self("exit66", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 66);
}
TC_CASE(handler_returns_still_dies)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'e') _set_invalid_parameter_handler(vexit); else _set_invalid_parameter_handler(vret);
        _invalid_parameter_noinfo_noreturn();
        return 77;
    }
    TC_RUN(handler_exits_66); TC_RUN(handler_returns_still_dies);
    return tc_end();
}
