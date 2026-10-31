/* TC
 * id:    exp_neg_probe_ucrt_gets_family_state
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 removed gets; MS docs gets_s
 * note:  PROBE: asserts only the secure forms are exported by ucrtbase (gets/_getws/_cgets believed inline or legacy_stdio_definitions-only; misspellings certainly absent). Controls: gets_s, _getws_s, _cgets_s.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void present(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (!p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void check_list(const char *const *v, size_t n, int want)
{
    size_t i;
    for (i = 0; i < n; i++) { if (want) present(v[i]); else absent(v[i]); }
}
#define N(a) (sizeof(a) / sizeof((a)[0]))
static const char *const l_secure_present[] = {
    "gets_s",
    "_getws_s",
    "_cgets_s",
};
static const char *const l_misspell_absent[] = {
    "_gets_s",
    "gets_",
    "_gets",
    "gets_s_",
    "_getws_",
};
static const char *const l_legacy_absent[] = {
    "gets",
    "_getws",
};
TC_CASE(secure_present) { CHECK(crt != NULL); check_list(l_secure_present, N(l_secure_present), 1); }
TC_CASE(misspell_absent) { CHECK(crt != NULL); check_list(l_misspell_absent, N(l_misspell_absent), 0); }
TC_CASE(legacy_absent) { CHECK(crt != NULL); check_list(l_legacy_absent, N(l_legacy_absent), 0); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(secure_present);
    TC_RUN(misspell_absent);
    TC_RUN(legacy_absent);
    return tc_end();
}
