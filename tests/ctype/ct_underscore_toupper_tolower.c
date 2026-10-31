/* TC
 * id:    ct_underscore_toupper_tolower
 * area:  ctype
 * funcs: _toupper _tolower
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _toupper, _tolower
 * note:  Only the defined domain (lowercase letter for _toupper, uppercase letter for _tolower) is tested.
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;

TC_CASE(_toupper_a_to_z_maps_to_A_to_Z) { int c; for (c = 'a'; c <= 'z'; c++) { v = c; CHECK_EQ_I((_toupper)(v), c - 32); } }
TC_CASE(_tolower_A_to_Z_maps_to_a_to_z) { int c; for (c = 'A'; c <= 'Z'; c++) { v = c; CHECK_EQ_I((_tolower)(v), c + 32); } }

int main(void) { tc_begin(); TC_RUN(_toupper_a_to_z_maps_to_A_to_Z); TC_RUN(_tolower_A_to_Z_maps_to_a_to_z); return tc_end(); }
