/* TC
 * id:    sec_neg_handler_null_restores_default
 * area:  secure_crt
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler: passing NULL restores the default (fatal) behaviour
 * note:  Fatal paths run in child processes. Only "non-zero and not the sentinel 77" is asserted for the cleared case.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <stdlib.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static void __cdecl quiet(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; }

static int child_main(const char *n)
{
    char d[2]; volatile size_t zero = 0; int r;
    iph_t (__cdecl *set)(iph_t) = (iph_t (__cdecl *)(iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (!set) return 90;
    if (tc_strequ(n, "installed")) {
        set(quiet);
        r = strcpy_s(d, zero, "x");
        return r != 0 ? 0 : 91;      /* handler returned, error code came back */
    }
    if (tc_strequ(n, "cleared")) {
        set(quiet);
        set(NULL);
        strcpy_s(d, zero, "x");      /* default handler: must not return */
        return 77;
    }
    return 92;
}

TC_CASE(child_handler_installed_returns_normally_exit_0)
{
    DWORD code = 1; int ran = tc_spawn_self("installed", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(child_handler_cleared_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("cleared", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(parent_exit_code_checks)
{
    DWORD code = 0; int ran = tc_spawn_self("no_such_child", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 92);   /* harness sanity: unknown child name reaches the fallthrough */
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_handler_installed_returns_normally_exit_0);
    TC_RUN(child_handler_cleared_terminates);
    TC_RUN(parent_exit_code_checks);
    return tc_end();
}
