/* TC
 * id:    scn_snscanf_percent_n_at_end_probe
 * area:  scanf
 * funcs: _snscanf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 (%n: number of characters consumed so far); MS docs _snscanf
 * note:  PROBE: %n evaluated when a conversion was ended by the count limit (end of input). The standard says
 *        %n yields the chars consumed (4); older CRTs may count the EOF read differently.
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(scanset_then_n) { char src[8] = {'a','b','c','d','e','f','g','h'}; char w[16]; int p = -1, r; w[0] = 0; cnt = 4; r = _snscanf(src, cnt, "%[a-z]%n", w, &p); CHECK_EQ_I(r, 1); CHECK_EQ_I(p, 4); }

int main(void) { tc_begin(); TC_RUN(scanset_then_n); return tc_end(); }
