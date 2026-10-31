/* TC
 * id:    scn_neg_snscanf_s_small_buf
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _snscanf_s: buffer too small fails the conversion
 * note:  Probes: _snscanf_s %s with size 2 for token abc returns 0, does not write past size 2.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(too_small) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; tc_iph_install(); v = "abc"; n = _snscanf_s((const char *)v, 3, "%s", a, (unsigned)2); CHECK_EQ_I(n, 0); CHECK_EQ_I(a[2], 'Z'); CHECK_EQ_I(a[7], 'Z'); }
int main(void) { tc_begin(); TC_RUN(too_small); return tc_end(); }
