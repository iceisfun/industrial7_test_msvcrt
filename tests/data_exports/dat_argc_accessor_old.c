/* TC
 * id:    dat_argc_accessor_old
 * area:  data_exports
 * kind:  positive
 * funcs: 
 * procs: __p___argc __p___argv
 * crt:   msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   C11 5.1.2.2.1; MS docs __argc, __argv
 * note:  PROBE: whether x64 msvcr80/msvcrt export __p___argc/__p___argv at all (cases skip if not). mingw import libs lack __p___argc for these CRTs, so resolved at run time.
 */
#include "tc.h"

TC_DECL_PROC(int *, __p___argc, (void));
TC_DECL_PROC(char ***, __p___argv, (void));

TC_CASE(argc_ge1) { if (!tc_p___p___argc) TC_SKIP("no accessor"); { int *p = tc_p___p___argc(); CHECK(p != NULL); if (p) CHECK(*p >= 1); } }
TC_CASE(argv0_nonempty) { char ***p; if (!tc_p___p___argv) TC_SKIP("no accessor"); p = tc_p___p___argv(); CHECK(p && *p && (*p)[0]); if (p && *p && (*p)[0]) CHECK((*p)[0][0] != 0); }
TC_CASE(argv_null_terminated)
{
    int *c; char ***v;
    if (!tc_p___p___argc || !tc_p___p___argv) TC_SKIP("no accessor");
    c = tc_p___p___argc(); v = tc_p___p___argv();
    if (!c || !v || !*v) { CHECK(c && v && *v); return; }
    CHECK((*v)[*c] == NULL);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p___argc); TC_LOAD_PROC(__p___argv);
    TC_RUN(argc_ge1); TC_RUN(argv0_nonempty); TC_RUN(argv_null_terminated);
    return tc_end();
}
