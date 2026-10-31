/* TC
 * id:    ext_probe_neg_signal_handler_sentinel_values
 * area:  exit_signal
 * funcs: signal
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs signal (SIG_ERR is the failure return value)
 * note:  PROBES that signal(SIGTERM, SIG_ERR) is accepted without crash and returns the previous handler (SIG_DFL),
 *        i.e. not SIG_ERR. No raise is performed. CRT internal sentinels (SIG_GET, SIG_SGE, SIG_ACK) are not touched.
 */
#include "tc.h"
#include <signal.h>

TC_CASE(sig_err_as_handler)
{
    typedef void (__cdecl *sh_t)(int);
    sh_t r, r2;
    if (TC_AT_LEAST(80)) tc_iph_install();
    r = signal(SIGTERM, SIG_ERR);
    CHECK(r != SIG_ERR);
    CHECK(r == SIG_DFL);
    r2 = signal(SIGTERM, SIG_DFL);
    (void)r2;
    CHECK(signal(SIGTERM, SIG_DFL) == SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(sig_err_as_handler); return tc_end(); }
