/* TC
 * id:    scn_sscanf_longdouble_l_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf width/size specifiers (long double is the same as double)
 * note:  PROBE: %Lf stores an 8-byte double on MS CRTs (not an 80-bit x87 value); sentinel bytes after 8 must be untouched. mingw long double is 80-bit so a raw struct is used.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

struct buf { double v; unsigned char s[8]; };

TC_CASE(L_is_8_bytes)
{
    struct buf b; int n;
    memset(&b, 0xAA, sizeof b); b.v = 0;
    n = sscanf("2.5", "%Lf", (void *)&b);
    CHECK_EQ_I(n, 1);
    CHECK_BITS_D(b.v, 2.5);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I(b.s[i], 0xAA); }
}
TC_CASE(lf_is_8_bytes_ref)
{
    struct buf b; int n;
    memset(&b, 0xAA, sizeof b);
    n = sscanf("2.5", "%lf", &b.v);
    CHECK_EQ_I(n, 1);
    CHECK_BITS_D(b.v, 2.5);
    { int i; for (i = 0; i < 8; i++) CHECK_EQ_I(b.s[i], 0xAA); }
}

int main(void) { tc_begin(); TC_RUN(L_is_8_bytes); TC_RUN(lf_is_8_bytes_ref); return tc_end(); }
