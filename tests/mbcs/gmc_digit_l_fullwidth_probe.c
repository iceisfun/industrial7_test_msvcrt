/* TC
 * id:    gmc_digit_l_fullwidth_probe
 * area:  mbcs
 * funcs: _ismbcdigit_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcdigit
 * note:  PROBE: fullwidth zero 0x824F is a digit for _ismbcdigit_l under cp932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(fw_zero) { CHECK_EQ_I(_setmbcp(932), 0); v = 0x824F; CHECK(_ismbcdigit_l(v, NULL) != 0); }
int main(void) { tc_begin(); TC_RUN(fw_zero); _setmbcp(-3); return tc_end(); }
