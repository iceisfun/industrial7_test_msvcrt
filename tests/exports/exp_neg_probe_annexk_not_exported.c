/* TC
 * id:    exp_neg_probe_annexk_not_exported
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 Annex K vs MS docs (MS uses _set_invalid_parameter_handler; strnlen_s is an inline in string.h)
 * note:  PROBE: Annex K names that MS does not implement as DLL exports. Controls are documented MS secure functions.
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
static const char *const l_control[] = {
    "asctime_s",
    "strerror_s",
    "qsort_s",
    "bsearch_s",
    "_set_invalid_parameter_handler",
};
static const char *const l_memset_s_absent[] = {
    "memset_s",
};
static const char *const l_nlen_absent[] = {
    "strnlen_s",
    "wcsnlen_s",
};
static const char *const l_handlers_absent[] = {
    "set_constraint_handler_s",
    "ignore_handler_s",
    "abort_handler_s",
};
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }
TC_CASE(memset_s_absent) { CHECK(crt != NULL); check_list(l_memset_s_absent, N(l_memset_s_absent), 0); }
TC_CASE(nlen_absent) { CHECK(crt != NULL); check_list(l_nlen_absent, N(l_nlen_absent), 0); }
TC_CASE(handlers_absent) { CHECK(crt != NULL); check_list(l_handlers_absent, N(l_handlers_absent), 0); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control);
    TC_RUN(memset_s_absent);
    TC_RUN(nlen_absent);
    TC_RUN(handlers_absent);
    return tc_end();
}
