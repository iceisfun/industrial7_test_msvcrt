/* TC
 * id:    cv_neg_wtoi_null_legacy_probe
 * area:  conv
 * funcs: _wtoi _wtol _wtoi64 _wtof
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atoi/_wtoi/atof/_wtof parameter validation (documented generically, not per CRT version)
 * note:  PROBING: that _wtoi/_wtol/_wtoi64/_wtof(NULL) on msvcr120..90 invoke the invalid parameter handler once, set errno EINVAL and return 0.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const wchar_t *volatile nul;

TC_CASE(wtoi_null) { int r; tc_iph_count = 0; errno = 0; r = _wtoi(nul); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wtol_null) { long r; tc_iph_count = 0; errno = 0; r = _wtol(nul); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
/* these two run in a child process so that a crash is reported without masking the other cases; child exits 10 on the documented behaviour */
static int child_main(const char *n)
{
    int ok;
    if (!tc_iph_install()) return 12;
    tc_iph_count = 0; errno = 0;
    if (tc_strequ(n, "wtoi64")) { __int64 r = _wtoi64(nul); ok = (r == 0); }
    else if (tc_strequ(n, "wtof")) { double r = _wtof(nul); ok = (tc_dbits(r) == 0); }
    else return 13;
    return (ok && errno == EINVAL && tc_iph_count == 1) ? 10 : 11;
}
static void run(const char *n) { DWORD code = 0; int ok = tc_spawn_self(n, &code, 20000); CHECK(ok); CHECK_EQ_U(code, 10); }
TC_CASE(wtoi64_null) { run("wtoi64"); }
TC_CASE(wtof_null) { run("wtof"); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(wtoi_null); TC_RUN(wtol_null); TC_RUN(wtoi64_null); TC_RUN(wtof_null);
    return tc_end();
}
