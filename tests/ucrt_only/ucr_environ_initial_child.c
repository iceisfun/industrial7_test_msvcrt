/* TC
 * id:    ucr_environ_initial_child
 * area:  ucrt_only
 * funcs:
 * procs: _initialize_narrow_environment _get_initial_narrow_environment _initialize_wide_environment _get_initial_wide_environment
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   corecrt_startup.h; MS docs _environ
 * note:  PROBE: _initialize_*_environment return 0 and _get_initial_*_environment return the process environment
 *        block as a NULL-terminated array of "name=value" strings. Parent sets TC_UCR_ENV=hello before spawning.
 */
#include "tc.h"

typedef int (__cdecl *init_t)(void);
typedef char **(__cdecl *getn_t)(void);
typedef wchar_t **(__cdecl *getw_t)(void);

static int child_main(void)
{
    init_t in = (init_t)tc_crt_proc("_initialize_narrow_environment"), iw = (init_t)tc_crt_proc("_initialize_wide_environment");
    getn_t gn = (getn_t)tc_crt_proc("_get_initial_narrow_environment");
    getw_t gw = (getw_t)tc_crt_proc("_get_initial_wide_environment");
    char **e; wchar_t **w; int found = 0, i, j;
    static const wchar_t want[] = L"TC_UCR_ENV=hello";
    if (!in || !iw || !gn || !gw) return 70;
    if (in() != 0) return 71;
    if (iw() != 0) return 72;
    e = gn(); if (!e) return 73;
    for (i = 0; e[i]; i++) {
        int eq = 0; for (j = 0; e[i][j]; j++) if (e[i][j] == '=') eq = 1;
        /* Windows may keep "=C:=..." drive entries which still contain '=' */
        if (!eq) return 74;
        if (tc_strequ(e[i], "TC_UCR_ENV=hello")) found = 1;
    }
    if (!found) return 75;
    found = 0;
    w = gw(); if (!w) return 76;
    for (i = 0; w[i]; i++) if (tc_wcsequ(w[i], want)) found = 1;
    if (!found) return 77;
    return 0;
}
TC_CASE(child)
{
    DWORD code = 99;
    SetEnvironmentVariableA("TC_UCR_ENV", "hello");
    CHECK(tc_spawn_self("env", &code, 20000)); CHECK_EQ_U(code, 0);
    SetEnvironmentVariableA("TC_UCR_ENV", NULL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(child);
    return tc_end();
}
