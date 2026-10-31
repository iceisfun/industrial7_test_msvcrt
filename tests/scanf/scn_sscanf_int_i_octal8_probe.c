/* TC
 * id:    scn_sscanf_int_i_octal8_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.2 (%i as strtol base 0)
 * note:  PROBES that "%i" on "08" reads the octal prefix "0" (value 0), stops before '8' and leaves '8' unread, on every
 *        CRT including old msvcrt.dll/msvcr80-100 whose %i implementation is not documented in detail.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(eight_not_octal) { int a = 5, n, pos = -1; char c = 0; v = "08"; n = sscanf((const char *)v, "%i%c%n", &a, &c, &pos); CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 0); CHECK_EQ_I(c, '8'); CHECK_EQ_I(pos, 2); }
int main(void) { tc_begin(); TC_RUN(eight_not_octal); return tc_end(); }
