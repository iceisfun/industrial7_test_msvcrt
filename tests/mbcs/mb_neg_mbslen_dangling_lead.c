/* TC
 * id:    mb_neg_mbslen_dangling_lead
 * area:  mbcs
 * funcs: _mbslen _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbslen: no validity check; lead byte followed by NUL
 * note:  Asserts only that the call returns (no crash, no read past terminator crash) and the count is at most 1.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(dangling) { static const unsigned char s[] = { 0x82, 0, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK(_mbslen(s) <= 1); }

int main(void) { tc_begin(); TC_RUN(dangling); _setmbcp(_MB_CP_ANSI); return tc_end(); }
