/* TC
 * id:    ct_neg_isctype_zero_mask
 * area:  ctype
 * funcs: _isctype iswctype
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _isctype, iswctype
 * note:  Mask 0 never matches. _LEADBYTE (0x8000) is a real mask bit; with no lead bytes in the C locale it still yields 0.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile int vc;
static volatile unsigned int vw;

TC_CASE(isctype_zero_mask_all_0_255) { int c, bad = 0; for (c = 0; c <= 255; c++) { vc = c; if (_isctype(vc, 0) != 0) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(isctype_EOF_zero_mask) { vc = -1; CHECK_EQ_I(_isctype(vc, 0), 0); }
TC_CASE(iswctype_zero_mask_0_255) { unsigned c; int bad = 0; for (c = 0; c <= 255; c++) { vw = c; if (iswctype(vw, 0) != 0) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(leadbyte_mask_0x8000_c_locale_zero) { int c, bad = 0; for (c = 0; c <= 255; c++) { vc = c; if (_isctype(vc, 0x8000) != 0) bad++; } CHECK_EQ_I(bad, 0); }

int main(void) { tc_begin(); TC_RUN(isctype_zero_mask_all_0_255); TC_RUN(isctype_EOF_zero_mask); TC_RUN(iswctype_zero_mask_0_255); TC_RUN(leadbyte_mask_0x8000_c_locale_zero); return tc_end(); }
