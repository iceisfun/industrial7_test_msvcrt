/* TC
 * id:    ucr_strtol_0x_alone_probe
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.1.4 (longest initial subsequence of the expected form: "0" is, "0x" without hex digits is not)
 * note:  PROBE: strtol("0x", &e, 16) returns 0 with e pointing at the 'x' (index 1), per the standard. Old MS CRTs may instead set e = nptr (no conversion) or past the 'x'; real Windows result wanted. Same for base 0 and for "0xg".
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
    CHECK_EQ_I(e - s, 1);
}
TC_CASE(base16) { one("0x", 16); }
TC_CASE(base0) { one("0x", 0); }
TC_CASE(nonhex_after) { one("0xg", 16); }
TC_CASE(unsigned_base16)
{
    char *e = NULL; v_s = "0x"; v_base = 16;
    CHECK_EQ_U(strtoul(v_s, &e, v_base), 0);
    CHECK_EQ_I(e - v_s, 1);
}
int main(void) { tc_begin(); TC_RUN(base16); TC_RUN(base0); TC_RUN(nonhex_after); TC_RUN(unsigned_base16); return tc_end(); }
