/* TC
 * id:    gmc_blank_ideographic_probe
 * area:  mbcs
 * funcs: _setmbcp
 * procs: _ismbcblank
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcblank
 * note:  PROBE: 0x8140 ideographic space is blank under cp932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

TC_DECL_PROC(int, _ismbcblank, (unsigned int));
static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(ideo) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x8140, tc_p__ismbcblank)); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ismbcblank); CHECK(tc_p__ismbcblank); TC_RUN(ideo); _setmbcp(-3); return tc_end(); }
