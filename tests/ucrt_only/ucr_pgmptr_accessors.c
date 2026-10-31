/* TC
 * id:    ucr_pgmptr_accessors
 * area:  ucrt_only
 * funcs: _get_pgmptr
 * procs: __p__pgmptr
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _pgmptr, _get_pgmptr (full path of the executable)
 */
#include "tc.h"
#include <stdlib.h>

typedef char **(__cdecl *pp_t)(void);
static pp_t pp;

static int ieq(const char *a, const char *b)
{ for (; *a && *b; a++, b++) { char x = *a, y = *b; if (x >= 'A' && x <= 'Z') x += 32; if (y >= 'A' && y <= 'Z') y += 32; if (x != y) return 0; } return *a == *b; }
static int endsexe(const char *s) { size_t n = 0; while (s[n]) n++; return n > 4 && ieq(s + n - 4, ".exe"); }

TC_CASE(narrow)
{
    char *p = NULL, mod[MAX_PATH];
    GetModuleFileNameA(NULL, mod, MAX_PATH);
    CHECK_EQ_I(_get_pgmptr(&p), 0);
    CHECK(p != NULL);
    if (p) { CHECK(ieq(p, mod)); CHECK(endsexe(p)); }
    if (pp) CHECK_EQ_P(*pp(), p);
}
int main(void)
{
    tc_begin();
    pp = (pp_t)tc_crt_proc("__p__pgmptr");
    TC_RUN(narrow);
    return tc_end();
}
