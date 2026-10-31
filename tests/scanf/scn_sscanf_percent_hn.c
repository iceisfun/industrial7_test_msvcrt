/* TC
 * id:    scn_sscanf_percent_hn
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2p12 (hn stores short); MS docs width specification (h)
 * note:  Probes that %hn stores a 16-bit value (neighbouring short must be untouched). C says so; not clearly documented for every CRT generation.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(hn_stores_short)
{
    short v[3]; v[0] = v[1] = v[2] = (short)0x7777;
    sscanf("hello", "hello%hn", &v[1]);
    CHECK_EQ_I(v[1], 5); CHECK_EQ_I(v[0], 0x7777); CHECK_EQ_I(v[2], 0x7777);
}
int main(void) { tc_begin(); TC_RUN(hn_stores_short); return tc_end(); }
