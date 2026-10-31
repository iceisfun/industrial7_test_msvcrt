/* TC
 * id:    exp_neg_probe_pre120_c99_stdlib_absent
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs C99 library support: added in VS2013 (msvcr120)
 * note:  PROBE: C99 library names believed absent before VS2013. snprintf/vsnprintf listed with VS2015 docs in mind; pre-2013 CRTs only had _snprintf/_vsnprintf.
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
    "_strtoi64",
    "_atoi64",
    "_vsnprintf",
    "_snprintf",
};
static const char *const l_conv_absent[] = {
    "strtoll",
    "strtoull",
    "wcstoll",
    "wcstoull",
    "atoll",
    "lldiv",
    "llabs",
    "imaxabs",
    "imaxdiv",
    "strtoimax",
    "strtoumax",
    "wcstoimax",
};
static const char *const l_stdio_absent[] = {
    "vfscanf",
    "vscanf",
    "vsscanf",
    "snprintf",
    "vsnprintf",
};
static const char *const l_misc_absent[] = {
    "isblank",
    "iswblank",
    "_Exit",
};
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }
TC_CASE(conv_absent) { CHECK(crt != NULL); check_list(l_conv_absent, N(l_conv_absent), 0); }
TC_CASE(stdio_absent) { CHECK(crt != NULL); check_list(l_stdio_absent, N(l_stdio_absent), 0); }
TC_CASE(misc_absent) { CHECK(crt != NULL); check_list(l_misc_absent, N(l_misc_absent), 0); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control);
    TC_RUN(conv_absent);
    TC_RUN(stdio_absent);
    TC_RUN(misc_absent);
    return tc_end();
}
