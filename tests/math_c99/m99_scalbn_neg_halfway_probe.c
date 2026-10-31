/* TC
 * id:    m99_scalbn_neg_halfway_probe
 * area:  math_c99
 * funcs: scalbn
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.3.13 (result rounded per current rounding mode)
 * note:  PROBE: scalbn(min subnormal, -1) is exactly halfway between 0 and 2^-1074; IEEE ties-to-even gives +0.
 *        Whether the Microsoft implementation rounds or truncates here is unverified.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#define ERR_OK() (errno == 0 || errno == ERANGE)
TC_CASE(min_half) { union { unsigned long long u; double d; } m; volatile double a; m.u = 1; a = m.d; errno = 0; CHECK_BITS_D(scalbn(a, -1), 0.0); CHECK(ERR_OK()); }
int main(void) { tc_begin(); TC_RUN(min_half); return tc_end(); }
