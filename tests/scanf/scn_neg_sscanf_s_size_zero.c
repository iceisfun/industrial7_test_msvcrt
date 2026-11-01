/* TC
 * id:    scn_neg_sscanf_s_size_zero
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs scanf_s: buffer size argument; a destination too small for the data invokes the invalid parameter handler
 * note:  PROBES: sscanf_s with buffer size 0 for %s/%c/%[ invokes the invalid parameter handler exactly once and writes nothing
 *        to the destination (not even a NUL). MS docs only say a too-small buffer is an error; size 0 specifics are unproven.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): size 0 for %s/%c/%[ does not call the invalid parameter handler (count 0) and writes nothing to the destination (sentinel intact). Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=90) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned vsz;

TC_CASE(s_zero)   { char b[4]; int i; for (i = 0; i < 4; i++) b[i] = 'Z'; vsz = 0; CHECK(tc_iph_install()); tc_iph_count = 0;
    sscanf_s("abc", "%s", b, vsz); CHECK_EQ_I(tc_iph_count, 0); for (i = 0; i < 4; i++) CHECK_EQ_I(b[i], 'Z'); }
TC_CASE(c_zero)   { char b[4]; int i; for (i = 0; i < 4; i++) b[i] = 'Z'; vsz = 0; CHECK(tc_iph_install()); tc_iph_count = 0;
    sscanf_s("abc", "%c", b, vsz); CHECK_EQ_I(tc_iph_count, 0); for (i = 0; i < 4; i++) CHECK_EQ_I(b[i], 'Z'); }
TC_CASE(set_zero) { char b[4]; int i; for (i = 0; i < 4; i++) b[i] = 'Z'; vsz = 0; CHECK(tc_iph_install()); tc_iph_count = 0;
    sscanf_s("abc", "%[a-z]", b, vsz); CHECK_EQ_I(tc_iph_count, 0); for (i = 0; i < 4; i++) CHECK_EQ_I(b[i], 'Z'); }

int main(void) { tc_begin(); TC_RUN(s_zero); TC_RUN(c_zero); TC_RUN(set_zero); return tc_end(); }
