/* TC
 * id:    dat_pgmptr_data_win7
 * area:  data_exports
 * kind:  positive
 * funcs: 
 * procs: _pgmptr
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _pgmptr (data export on Win7-era CRTs)
 * note:  PROBE: _pgmptr data export exists in each listed CRT (incl. x64 msvcr80/msvcrt) and its basename equals GetModuleFileNameA's. Docs allow a bare program name when not run from a command line. Data export resolved with GetProcAddress.
 */
#include "tc.h"

static char lc(char c) { return (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c; }
static const char *base_of(const char *s) { const char *b = s; for (; *s; s++) if (*s == '\\' || *s == '/') b = s + 1; return b; }
static int eq_ci(const char *a, const char *b) { while (*a && lc(*a) == lc(*b)) { a++; b++; } return lc(*a) == lc(*b); }

TC_CASE(pgmptr_nonnull) { char **p = (char **)tc_crt_proc("_pgmptr"); CHECK(p != NULL); if (p) { CHECK(*p != NULL); if (*p) CHECK((*p)[0] != 0); } }
TC_CASE(pgmptr_matches_module_name)
{
    char **p = (char **)tc_crt_proc("_pgmptr"); char exe[MAX_PATH];
    if (!p || !*p) { CHECK(p && *p); return; }
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    CHECK(eq_ci(base_of(*p), base_of(exe)));
}
int main(void) { tc_begin(); TC_RUN(pgmptr_nonnull); TC_RUN(pgmptr_matches_module_name); return tc_end(); }
