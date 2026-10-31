/* TC
 * id:    gmc_space_l_ideographic_probe
 * area:  mbcs
 * funcs: _ismbcspace_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcspace
 * note:  PROBE: 0x8140 ideographic space is space under cp932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(ideo) { CHECK_EQ_I(_setmbcp(932), 0); v = 0x8140; CHECK(_ismbcspace_l(v, NULL) != 0); }
int main(void) { tc_begin(); TC_RUN(ideo); _setmbcp(-3); return tc_end(); }
