/* TC
 * id:    dat_pgmptr_path_probe
 * area:  data_exports
 * kind:  positive
 * funcs: _get_pgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _pgmptr
 * note:  PROBE: for a process started via CreateProcess with a full image path, _pgmptr is a full path (drive-absolute, UNC or \\?\ prefixed) whose basename equals GetModuleFileNameA's (case-insensitive). Docs only say it MAY be a bare name or relative path when not run from a command line.
 */
#include "tc.h"
#include <stdlib.h>

static char lc(char c) { return (c >= 'A' && c <= 'Z') ? (char)(c + 32) : c; }
static const char *base_of(const char *s) { const char *b = s; for (; *s; s++) if (*s == '\\' || *s == '/') b = s + 1; return b; }
static int eq_ci(const char *a, const char *b) { while (*a && lc(*a) == lc(*b)) { a++; b++; } return lc(*a) == lc(*b); }

TC_CASE(path_nonempty_absolute)
{
    char *p = NULL; _get_pgmptr(&p);
    if (!p) { CHECK(p != NULL); return; }
    CHECK(p[0] != 0);
    CHECK((p[1] == ':' && (p[2] == '\\' || p[2] == '/')) || (p[0] == '\\' && p[1] == '\\'));
}
TC_CASE(matches_GetModuleFileNameA_basename)
{
    char *p = NULL; char exe[MAX_PATH]; _get_pgmptr(&p);
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    if (!p) { CHECK(p != NULL); return; }
    CHECK(eq_ci(base_of(p), base_of(exe)));
}
int main(void) { tc_begin(); TC_RUN(path_nonempty_absolute); TC_RUN(matches_GetModuleFileNameA_basename); return tc_end(); }
