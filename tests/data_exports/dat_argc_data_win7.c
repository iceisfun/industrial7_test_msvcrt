/* TC
 * id:    dat_argc_data_win7
 * area:  data_exports
 * kind:  positive
 * funcs: 
 * procs: __argc __argv __p___argc __p___argv
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs __argc, __argv, __wargv (data exports of the Win7-era CRTs)
 * note:  PROBE: that each listed CRT (esp. x64 msvcr80/msvcrt) exports __argc/__argv as data and populates them for a narrow main(). The accessor-equality cases are skipped when the CRT lacks the accessor (x64 msvcr80/msvcrt may not export __p___argc). Data exports are resolved with GetProcAddress (address of the variable). ucrtbase removed them: see dat_neg_ucrt_data_not_exported.
 */
#include "tc.h"

TC_DECL_PROC(int *, __p___argc, (void));
TC_DECL_PROC(char ***, __p___argv, (void));
static int *p_argc; static char ***p_argv;

TC_CASE(argc_ge1) { CHECK(p_argc != NULL); if (p_argc) CHECK(*p_argc >= 1); }
TC_CASE(argv0_nonempty) { CHECK(p_argv && *p_argv && (*p_argv)[0]); if (p_argv && *p_argv && (*p_argv)[0]) CHECK((*p_argv)[0][0] != 0); }
TC_CASE(argc_matches_accessor) { if (!tc_p___p___argc) TC_SKIP("accessor not exported"); if (!p_argc) { CHECK(p_argc != NULL); return; } CHECK_EQ_I(*p_argc, *tc_p___p___argc()); }
TC_CASE(argv_matches_accessor) { if (!tc_p___p___argv) TC_SKIP("accessor not exported"); if (!p_argv) { CHECK(p_argv != NULL); return; } CHECK_EQ_P(*p_argv, *tc_p___p___argv()); }
int main(void)
{
    tc_begin();
    p_argc = (int *)tc_crt_proc("__argc"); p_argv = (char ***)tc_crt_proc("__argv");
    TC_LOAD_PROC(__p___argc); TC_LOAD_PROC(__p___argv);
    TC_RUN(argc_ge1); TC_RUN(argv0_nonempty); TC_RUN(argc_matches_accessor); TC_RUN(argv_matches_accessor);
    return tc_end();
}
