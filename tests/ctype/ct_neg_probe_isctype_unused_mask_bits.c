/* TC
 * id:    ct_neg_probe_isctype_unused_mask_bits
 * area:  ctype
 * funcs: _isctype iswctype
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isctype / iswctype (mask bits _UPPER 1 .. _ALPHA 0x100 defined)
 * note:  PROBE: bits 0x1000, 0x2000, 0x4000 are not assigned to any class in ctype.h; no ASCII char matches them.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static void sweep(int mask)
{
    int c;
    for (c = -1; c < 128; c++) { volatile int v = c; CHECK_EQ_I(_isctype(v, mask), 0); }
    for (c = 0; c < 128; c++) { volatile wint_t v = (wint_t)c; CHECK_EQ_I(iswctype(v, (wctype_t)mask), 0); }
}
TC_CASE(bit1000) { sweep(0x1000); }
TC_CASE(bit2000) { sweep(0x2000); }
TC_CASE(bit4000) { sweep(0x4000); }
TC_CASE(combined) { sweep(0x7000); }
int main(void) { tc_begin(); TC_RUN(bit1000); TC_RUN(bit2000); TC_RUN(bit4000); TC_RUN(combined); return tc_end(); }
