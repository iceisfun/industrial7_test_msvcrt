/* TC
 * id:    exp_neg_conio_plain_names_absent
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _getch/_kbhit/_putch/_cputs/_ungetch (POSIX names getch etc. are deprecated oldnames, not DLL exports)
 * note:  Plain getch/kbhit/... are supplied by oldnames.lib as wrappers around the underscore exports; the DLLs export only the underscore forms (observed from export tables, not documented). msvcrt.dll is covered by exp_neg_probe_msvcrt_oldnames_plain. _cget/_cgetw/_getchw are not CRT functions at all.
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
static const char *const l_not_real[] = {
    "_cget",
    "_cgetw",
    "_getchw",
};
static const char *const l_plain_absent[] = {
    "getch",
    "getche",
    "kbhit",
    "putch",
    "ungetch",
    "cputs",
    "cgets",
    "cprintf",
    "cscanf",
    "getwch",
    "putwch",
};
static const char *const l_control[] = {
    "_getch",
    "_kbhit",
    "_cputs",
    "_putch",
};
TC_CASE(not_real) { CHECK(crt != NULL); check_list(l_not_real, N(l_not_real), 0); }
TC_CASE(plain_absent) { CHECK(crt != NULL); check_list(l_plain_absent, N(l_plain_absent), 0); }
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(not_real);
    TC_RUN(plain_absent);
    TC_RUN(control);
    return tc_end();
}
