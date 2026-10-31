/* TC
 * id:    pfm_3_conv_e_len_ii_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes (I, I32, I64, L, j, z, t)
 * note:  PROBING: %Le %lle %Ie %I32e %I64e %je %ze %te on a double argument all format it like plain %e (modifiers ignored for floating types, L = long double = double on MS) and consume exactly one double (trailing %d must still print 7).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.500000e+00|7");
TC_CASE(Le) { BODY_T("%Le") }
TC_CASE(lle) { BODY_T("%lle") }
TC_CASE(Ie) { BODY_T("%Ie") }
TC_CASE(I32e) { BODY_T("%I32e") }
TC_CASE(I64e) { BODY_T("%I64e") }
TC_CASE(je) { BODY_T("%je") }
TC_CASE(ze) { BODY_T("%ze") }
TC_CASE(te) { BODY_T("%te") }
int main(void) { tc_begin(); TC_RUN(Le); TC_RUN(lle); TC_RUN(Ie); TC_RUN(I32e); TC_RUN(I64e); TC_RUN(je); TC_RUN(ze); TC_RUN(te); return tc_end(); }
