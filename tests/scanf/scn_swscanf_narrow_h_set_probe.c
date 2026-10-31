/* TC
 * id:    scn_swscanf_narrow_h_set_probe
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs scanf width/type: h prefix in wscanf
 * note:  PROBE: %h[a-z] in swscanf stores a narrow-char scanset string (docs list h only for c/s explicitly; assumed to extend to [).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(hset) { char b[16]; int r, k = -1, i; for (i = 0; i < 16; i++) b[i] = 'Z'; v = L"abcXYZ";
    r = swscanf((const wchar_t *)v, L"%h[a-z]%n", b, &k); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); CHECK_EQ_I(k, 3); CHECK_EQ_I(b[4], 'Z'); }
int main(void) { tc_begin(); TC_RUN(hset); return tc_end(); }
