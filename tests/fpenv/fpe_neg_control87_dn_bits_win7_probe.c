/* TC
 * id:    fpe_neg_control87_dn_bits_win7_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _control87
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _control87 (_MCW_DN, _DN_FLUSH, _DN_SAVE)
 * note:  PROBE: DN field requests on Win7-era CRTs, x64. Readback of _DN_FLUSH is logged only (may be ignored). Asserted: EM and RC untouched, and restoring _DN_SAVE brings the DN field back to its starting value.
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(dn_request)
{
    unsigned orig = _control87(0, 0), a, b;
    unsigned keep = _MCW_EM | _MCW_RC;
    a = _control87(_DN_FLUSH, _MCW_DN);
    logx("after_flush", a);
    b = _control87(0, 0);
    CHECK_EQ_U(b & keep, orig & keep);
    _control87(_DN_SAVE, _MCW_DN);
    b = _control87(0, 0);
    CHECK_EQ_U(b & _MCW_DN, orig & _MCW_DN);
    CHECK_EQ_U(b & keep, orig & keep);
    _control87(orig, _MCW_DN);
}
int main(void) { tc_begin(); TC_RUN(dn_request); return tc_end(); }
