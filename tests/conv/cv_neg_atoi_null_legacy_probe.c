/* TC
 * id:    cv_neg_atoi_null_legacy_probe
 * area:  conv
 * funcs: atoi atol _atoi64 atof
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs atoi/atof Parameter validation
 * note:  PROBE: older CRTs treat NULL to atoi/atol/_atoi64/atof as an invalid parameter (handler called once, errno EINVAL,
 *        return 0) rather than crashing. Each function runs in a child process so a crash is just a failed case.
 *        Child exit codes: 0 = all as expected, 1 = return value wrong, 2 = errno wrong, 3 = handler count wrong.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile nul;

static int chk(int zero, int iph) { if (!zero) return 1; if (errno != EINVAL) return 2; if (iph != 1) return 3; return 0; }

TC_CASE(atoi_null) { DWORD code = 99; CHECK(tc_spawn_self("atoi", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(atol_null) { DWORD code = 99; CHECK(tc_spawn_self("atol", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(i64_null)  { DWORD code = 99; CHECK(tc_spawn_self("i64", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(atof_null) { DWORD code = 99; CHECK(tc_spawn_self("atof", &code, 20000)); CHECK_EQ_U(code, 0); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int z = 0;
        if (!tc_iph_install()) return 50;
        tc_iph_count = 0; errno = 0;
        if (c[0] == 'a' && c[1] == 't' && c[2] == 'o' && c[3] == 'i') z = (atoi(nul) == 0);
        else if (c[0] == 'a' && c[1] == 't' && c[2] == 'o' && c[3] == 'l') z = (atol(nul) == 0);
        else if (c[0] == 'i') z = (_atoi64(nul) == 0);
        else z = (tc_dbits(atof(nul)) == 0);
        return chk(z, tc_iph_count);
    }
    TC_RUN(atoi_null); TC_RUN(atol_null); TC_RUN(i64_null); TC_RUN(atof_null);
    return tc_end();
}
