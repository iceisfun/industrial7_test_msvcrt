/* TC
 * id:    ucr_strtol_0x_alone_probe
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.22.1.4 (longest initial subsequence of the expected form: "0" is, "0x" without hex digits is not)
 * note:  Win10 22H2 ucrtbase (observed): strtol/strtoul of "0x" / "0xg" (base 16 and base 0) return 0 with endptr == nptr (offset 0); deviates from C11 7.22.1.4,
 *        whose longest-subject-sequence rule gives "0" and endptr at the 'x' (offset 1). Other CRTs not verified, hence crt: ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;
static volatile int v_base;

static void one(const char *s, int base)
{
    char *e = NULL; long r;
    v_s = s; v_base = base;
    r = strtol(v_s, &e, v_base);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(e - s, 0);   /* observed: no conversion performed, endptr = nptr (C11 would give 1) */
}
TC_CASE(base16) { one("0x", 16); }
TC_CASE(base0) { one("0x", 0); }
TC_CASE(nonhex_after) { one("0xg", 16); }
TC_CASE(unsigned_base16)
{
    char *e = NULL; v_s = "0x"; v_base = 16;
    CHECK_EQ_U(strtoul(v_s, &e, v_base), 0);
    CHECK_EQ_I(e - v_s, 0);
}
int main(void) { tc_begin(); TC_RUN(base16); TC_RUN(base0); TC_RUN(nonhex_after); TC_RUN(unsigned_base16); return tc_end(); }
