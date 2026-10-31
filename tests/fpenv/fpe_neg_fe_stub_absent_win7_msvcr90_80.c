/* TC
 * id:    fpe_neg_fe_stub_absent_win7_msvcr90_80
 * area:  fpenv
 * kind:  negative
 * funcs: 
 * procs: _controlfp _control87 _clearfp _statusfp fegetenv fesetenv feholdexcept fegetexceptflag fesetexceptflag feupdateenv fegetround
 * crt:   msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fenv.h requirements (C99 fenv from VS2013); _controlfp family exists everywhere
 * note:  PROBE: msvcr90/80/msvcrt.dll do not export the C99 fenv functions; the legacy _controlfp family is present.
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

static const char *const fe[] = { "fegetenv", "fesetenv", "feholdexcept", "fegetexceptflag", "fesetexceptflag", "feupdateenv", "fegetround" };
TC_CASE(fe_functions_absent)
{
    unsigned i;
    for (i = 0; i < sizeof fe / sizeof fe[0]; i++) if (tc_crt_proc(fe[i]) != NULL) tc_fail_expr(__FILE__, __LINE__, fe[i]);
}
TC_CASE(legacy_present)
{
    CHECK(tc_crt_proc("_controlfp") != NULL); CHECK(tc_crt_proc("_control87") != NULL);
    CHECK(tc_crt_proc("_clearfp") != NULL); CHECK(tc_crt_proc("_statusfp") != NULL);
}
int main(void) { tc_begin(); TC_RUN(fe_functions_absent); TC_RUN(legacy_present); return tc_end(); }
