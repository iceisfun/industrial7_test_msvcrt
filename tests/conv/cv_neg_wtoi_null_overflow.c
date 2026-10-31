/* TC
 * id:    cv_neg_wtoi_null_overflow
 * area:  conv
 * funcs: _wtoi _wtol _wtoi64 _wtof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi/_wtoi/atof/_wtof: NULL str invokes the invalid parameter handler, returns 0, errno EINVAL; overflow returns INT_MAX/INT_MIN with ERANGE (UCRT); legacy CRTs are covered by cv_neg_wtoi_null_legacy_probe
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

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
#ifdef TC_UCRT
TC_CASE(wtoi_overflow)
{
    static wchar_t *volatile big = L"2147483648", *volatile nbig = L"-2147483649";
    int r;
    errno = 0; r = _wtoi(big); CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; r = _wtoi(nbig); CHECK_EQ_I(r, INT_MIN); CHECK_EQ_I(errno, ERANGE);
}
#endif

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(wtoi_null); TC_RUN(wtol_null); TC_RUN(wtoi64_null); TC_RUN(wtof_null);
#ifdef TC_UCRT
    TC_RUN(wtoi_overflow);
#endif
    return tc_end();
}
