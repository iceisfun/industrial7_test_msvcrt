/* TC
 * id:    dat_cmdln_accessors
 * area:  data_exports
 * kind:  positive
 * funcs: __p__acmdln
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _acmdln is not documented; C runtime internals
 * note:  PROBE: _acmdln content (quoted or not, argv[0] vs full path) is undocumented; only asserts non-empty and containing the exe basename (without .exe), case-insensitive.
 */
#include "tc.h"

extern char **__cdecl __p__acmdln(void);

static char lc(char c) { return (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c; }
static int contains_ci(const char *h, const char *n)
{
    int i, j;
    for (i = 0; h[i]; i++) { for (j = 0; n[j] && h[i + j] && lc(h[i + j]) == lc(n[j]); j++) ; if (!n[j]) return 1; }
    return 0;
}
static void basename_noext(char *out)
{
    char exe[MAX_PATH]; int i, b = 0, n = 0;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    for (i = 0; exe[i]; i++) if (exe[i] == '\\' || exe[i] == '/') b = i + 1;
    for (i = b; exe[i] && n < MAX_PATH - 1; i++) out[n++] = exe[i];
    out[n] = 0;
    while (n > 0 && out[n - 1] != '.') n--;
    if (n > 0) out[n - 1] = 0;
}
TC_CASE(acmdln_nonnull_nonempty) { char **p = __p__acmdln(); CHECK(p != NULL); if (p) { CHECK(*p != NULL); if (*p) CHECK((*p)[0] != 0); } }
TC_CASE(acmdln_contains_exe_basename)
{
    char **p = __p__acmdln(); char b[MAX_PATH];
    if (!p || !*p) { CHECK(p && *p); return; }
    basename_noext(b);
    CHECK(b[0] != 0);
    CHECK(contains_ci(*p, b));
}
int main(void) { tc_begin(); TC_RUN(acmdln_nonnull_nonempty); TC_RUN(acmdln_contains_exe_basename); return tc_end(); }
