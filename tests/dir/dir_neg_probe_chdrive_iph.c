/* TC
 * id:    dir_neg_probe_chdrive_iph
 * area:  dir
 * kind:  negative
 * funcs: _chdrive _getdrives
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  Also probes errno == EACCES for out-of-range drive numbers.
 *        Probes whether _chdrive treats an out-of-range drive number (0, 27) as an invalid parameter (iph count 1) while
 *        an in-range but absent drive does not (iph count 0, plain SetCurrentDirectory failure).
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

TC_CASE(zero) { volatile int v = 0; tc_iph_install(); tc_iph_count = 0; _chdrive(v); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(twentyseven) { volatile int v = 27; tc_iph_install(); tc_iph_count = 0; _chdrive(v); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(errno_eacces) { volatile int v = 27; tc_iph_install(); errno = 0; CHECK_EQ_I(_chdrive(v), -1); CHECK_EQ_I(errno, EACCES); }
TC_CASE(absent)
{
    unsigned long m = _getdrives(); int d, pick = 0;
    tc_iph_install();
    for (d = 26; d >= 1; d--) if (!((m >> (d - 1)) & 1)) { pick = d; break; }
    if (!pick) TC_SKIP("all drive letters in use");
    tc_iph_count = 0; _chdrive(pick);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(errno_eacces); TC_RUN(twentyseven); TC_RUN(absent); return tc_end(); }
