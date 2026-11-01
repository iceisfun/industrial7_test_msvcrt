/* TC
 * id:    scn_sscanf_ll_overflow_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.2p10; MS docs scanf size prefixes (I64)
 * note:  PROBE: 64-bit wrap. Uses %I64 prefix, supported by every MS CRT.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): %I64d / %I64u clamp (strtoll-style) instead of wrapping: 2^63 -> LLONG_MAX, 2^64 -> ULLONG_MAX, -(2^63)-1 -> LLONG_MIN; n==1 and errno not asserted. The old wrap expectation was wrong.
 *        C11 7.21.6.2p10 makes out-of-range values undefined, so any choice is conforming.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
#ifdef TC_UCRT
TC_CASE(lld) { long long a = 5; int n; v = "9223372036854775808"; n = sscanf((const char *)v, "%I64d", &a); CHECK_EQ_I(n, 1); CHECK(a == LLONG_MAX); }
TC_CASE(lld_neg) { long long a = 5; int n; v = "-9223372036854775809"; n = sscanf((const char *)v, "%I64d", &a); CHECK_EQ_I(n, 1); CHECK(a == LLONG_MIN); }
TC_CASE(llu) { unsigned long long a = 5; int n; v = "18446744073709551616"; n = sscanf((const char *)v, "%I64u", &a); CHECK_EQ_I(n, 1); CHECK(a == ULLONG_MAX); }
int main(void) { tc_begin(); TC_RUN(lld); TC_RUN(llu); TC_RUN(lld_neg); return tc_end(); }
#else
TC_CASE(lld) { long long a = 5; int n; v = "9223372036854775808"; n = sscanf((const char *)v, "%I64d", &a); CHECK_EQ_I(n, 1); CHECK(a == LLONG_MIN); }
TC_CASE(llu) { unsigned long long a = 5; int n; v = "18446744073709551616"; n = sscanf((const char *)v, "%I64u", &a); CHECK_EQ_I(n, 1); CHECK(a == 0); }
int main(void) { tc_begin(); TC_RUN(lld); TC_RUN(llu); return tc_end(); }
#endif
