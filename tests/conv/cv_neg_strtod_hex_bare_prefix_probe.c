/* TC
 * id:    cv_neg_strtod_hex_bare_prefix_probe
 * area:  conv
 * funcs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3 (longest valid initial subsequence)
 * note:  Probes "0x" / "0xp3" (hex prefix with no hex digits): expected to convert only the leading "0" (value 0.0, endptr s+1)
 *        as C requires. Whether UCRT backtracks to the "0" is not documented.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile vs;
TC_CASE(bare_prefix)
{
    char *e; double d;
    vs = "0x";   d = strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vs + 1);
    vs = "0xp3"; d = strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vs + 1);
}
int main(void) { tc_begin(); TC_RUN(bare_prefix); return tc_end(); }
