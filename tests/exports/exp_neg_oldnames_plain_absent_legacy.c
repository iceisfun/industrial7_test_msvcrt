/* TC
 * id:    exp_neg_oldnames_plain_absent_legacy
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs: POSIX-named functions deprecated; oldnames.lib maps them to underscore exports
 * note:  Plain oldnames aliases are link-time only; the DLL exports the underscore names. msvcrt.dll is covered by a separate probe.
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
static const char *const l_str_absent[] = {
    "strlwr",
    "strupr",
    "strrev",
    "strset",
    "strnset",
    "strcmpi",
};
static const char *const l_wcs_absent[] = {
    "wcsdup",
    "wcsicmp",
    "wcslwr",
    "wcsupr",
    "wcsnicmp",
    "wcsrev",
    "wcsset",
};
static const char *const l_conv_absent[] = {
    "itoa",
    "ltoa",
    "ultoa",
    "gcvt",
    "ecvt",
    "fcvt",
};
static const char *const l_bessel_absent[] = {
    "j0",
    "j1",
    "jn",
    "y0",
    "y1",
    "yn",
};
static const char *const l_misc_absent[] = {
    "fdopen",
    "fgetchar",
    "fputchar",
    "fcloseall",
    "getw",
    "putw",
    "tempnam",
    "tell",
    "eof",
    "filelength",
    "setmode",
    "sopen",
    "mktemp",
    "tzset",
    "putenv",
    "swab",
    "onexit",
};
static const char *const l_control[] = {
    "_strlwr",
    "_itoa",
    "_j0",
    "_fdopen",
    "_tzset",
    "_putenv",
    "_swab",
};
TC_CASE(str_absent) { CHECK(crt != NULL); check_list(l_str_absent, N(l_str_absent), 0); }
TC_CASE(wcs_absent) { CHECK(crt != NULL); check_list(l_wcs_absent, N(l_wcs_absent), 0); }
TC_CASE(conv_absent) { CHECK(crt != NULL); check_list(l_conv_absent, N(l_conv_absent), 0); }
TC_CASE(bessel_absent) { CHECK(crt != NULL); check_list(l_bessel_absent, N(l_bessel_absent), 0); }
TC_CASE(misc_absent) { CHECK(crt != NULL); check_list(l_misc_absent, N(l_misc_absent), 0); }
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(str_absent);
    TC_RUN(wcs_absent);
    TC_RUN(conv_absent);
    TC_RUN(bessel_absent);
    TC_RUN(misc_absent);
    TC_RUN(control);
    return tc_end();
}
