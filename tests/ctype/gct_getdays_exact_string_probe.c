/* TC
 * id:    gct_getdays_exact_string_probe
 * area:  ctype
 * funcs: _Getdays
 * procs: free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   internal helper used by C++ std::time_get
 * note:  PROBE: exact formatting of the C-locale string, including leading ':' and no trailing ':'. Length is 85.
 */
#include "tc.h"
extern char *__cdecl _Getdays(void);
TC_DECL_PROC(void, free, (void *));
static const char want[] = ":Sun:Sunday:Mon:Monday:Tue:Tuesday:Wed:Wednesday:Thu:Thursday:Fri:Friday:Sat:Saturday";
TC_CASE(exact)
{
    char *d = _Getdays(); size_t n = 0;
    CHECK(d != NULL); if (!d) return;
    CHECK_STR(d, want);
    while (d[n]) n++;
    CHECK_EQ_U(n, sizeof want - 1);
    CHECK_EQ_U(n, 85);
    if (tc_p_free) tc_p_free(d);
}
int main(void) { tc_begin(); TC_LOAD_PROC(free); TC_RUN(exact); return tc_end(); }
