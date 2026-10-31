/* TC
 * id:    exp_neg_probe_msvcrt_oldnames_plain
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   none: belief that msvcrt.dll, like later CRTs, exports only underscore forms of oldnames
 * note:  PROBE: asserts msvcrt.dll does NOT export the plain POSIX/oldnames aliases (strlwr itoa j0 fdopen tzset ...). Unsure for any individual name; a failure lists which plain names exist. Underscore controls are certain.
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
    "_strlwr",
    "_itoa",
    "_j0",
    "_fdopen",
    "_tzset",
    "_putenv",
    "_swab",
};
static const char *const l_plain_posix_absent[] = {
    "strlwr",
    "strupr",
    "strrev",
    "strset",
    "strnset",
    "strcmpi",
    "itoa",
    "ltoa",
    "ultoa",
    "gcvt",
    "ecvt",
    "fcvt",
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
static const char *const l_conio_absent[] = { "getch", "getche", "kbhit", "putch", "ungetch", "cputs", "cgets", "cprintf", "cscanf", "getwch", "putwch" };
TC_CASE(conio_absent) { CHECK(crt != NULL); check_list(l_conio_absent, N(l_conio_absent), 0); }
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }
TC_CASE(plain_posix_absent) { CHECK(crt != NULL); check_list(l_plain_posix_absent, N(l_plain_posix_absent), 0); }
TC_CASE(wcs_absent) { CHECK(crt != NULL); check_list(l_wcs_absent, N(l_wcs_absent), 0); }
TC_CASE(bessel_absent) { CHECK(crt != NULL); check_list(l_bessel_absent, N(l_bessel_absent), 0); }
TC_CASE(misc_absent) { CHECK(crt != NULL); check_list(l_misc_absent, N(l_misc_absent), 0); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(control);
    TC_RUN(plain_posix_absent);
    TC_RUN(wcs_absent);
    TC_RUN(bessel_absent);
    TC_RUN(misc_absent);
    TC_RUN(conio_absent);
    return tc_end();
}
